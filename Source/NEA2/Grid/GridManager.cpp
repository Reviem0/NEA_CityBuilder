// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	PopulateGrid();
	PopulateGridNeighbours();
	SetNeighbourArray();
	InitGameManager();

}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridManager::PopulateGrid() {
	
	// Empty Array
	GridArray.Empty(); 
	
	// Total Grid Size
	int GridSize = GridSizeX * GridSizeY;
	
	// Calculate the center offset
	float CenterOffsetY = ((GridSizeX-1) * WorldGridSize) / 2.0f; 
	float CenterOffsetX = ((GridSizeY-1) * WorldGridSize) / 2.0f;

	//Grid Spawn Parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform Transform;
	FVector GridPos = FVector::ZeroVector;
	FVector GridScale = GetGridScale();
	GridPos.Z = HeightOffset;

	// Spawn Grid
	for (int i = 0; i < GridSizeY; i++) {
		for (int j = 0; j < GridSizeX; j++) {
			// Calculate Spawn Point of Grid Cell
			GridPos.X = (i * WorldGridSize) - CenterOffsetX; 
			GridPos.Y = (j * WorldGridSize) - CenterOffsetY;
			Transform = UKismetMathLibrary::MakeTransform(GridPos, FRotator::ZeroRotator, GridScale);
			
			// Spawn Grid Cell
			AGridCell* Grid = GetWorld()->SpawnActor<AGridCell>(GridCell, Transform, SpawnParams);
			Grid->SetActorRelativeScale3D(GridScale);
			
			// Add to grid array
			GridArray.Add(Grid);
		}
	}
}

// Get the position of the closest Grid from an input position
FVector AGridManager::GetClosestGridPosition(FVector InPosition)
{
	FVector ClosestPosition = GridArray[0]->GetActorLocation();
	float ClosestDistance = FVector::Dist(InPosition, ClosestPosition);

	// Search for closest Grid Cell to inPosition
	for (int i = 0; i < GridArray.Num(); i++) {
		AGridCell* Grid = GridArray[i];
		if (FVector::Dist(InPosition, Grid->GetActorLocation()) < ClosestDistance) {
			ClosestPosition = Grid->GetActorLocation();
			ClosestDistance = FVector::Dist(InPosition, ClosestPosition);
		}
	}

    return ClosestPosition;
}

// Get the closest Grid from an input position
AGridCell* AGridManager::GetClosestGridCell(FVector InPosition)
{
	if (GridArray.Num() == 0) {return 0;}
	int ClosestIndexY = 0;
	int ClosestIndexX = 0;
	float ClosestDistance = FVector::Dist(InPosition, GridArray[ClosestIndexY]->GetActorLocation());
	// Search for closest Grid Cell to inPosition
	for (int i = 0; i < GridArray.Num(); i = i + GridSizeX) {
		AGridCell* Grid = GridArray[i];
		float CurrentDistance = FVector::Dist(InPosition, Grid->GetActorLocation());
		if (CurrentDistance <= ClosestDistance) {
			ClosestIndexY = i;
			ClosestDistance = CurrentDistance;
		} else {
			break;
		}
	}
	for (int j = 0; j < GridSizeX; j++) {
		AGridCell* Grid = GridArray[ClosestIndexY + j];
		float CurrentDistance = FVector::Dist(InPosition, Grid->GetActorLocation());
		if (CurrentDistance <= ClosestDistance) {
			ClosestIndexX = ClosestIndexY + j;
			ClosestDistance = CurrentDistance;
		} else {
			break;
		}
	}

	return GridArray[ClosestIndexX];
}



// Return Grid Scale
FVector AGridManager::GetGridScale() 
{
	return FVector(WorldGridSize/100, WorldGridSize/100, WorldGridSize/100);
}

// Assign Grid Cells their Neighbours
void AGridManager::PopulateGridNeighbours() 
{
	for (int i = 0; i < GridArray.Num(); i++) {
		AGridCell* Grid = GridArray[i];

		// Get East Neighbour
		if ((i+1) % GridSizeX != 0 ) {
			Grid->ENeighbour = &(GridArray[i + 1]);
		}

		// Get West Neighbour
		if (i % GridSizeX != 0 && i != 0) {
			 Grid->WNeighbour = &(GridArray[i - 1]);
		}

		// Get North Neighbour
		if (i + GridSizeX < GridArray.Num()) {
			Grid->NNeighbour = &GridArray[i + GridSizeX];
		}

		// Get South Neighbour
		if (i - GridSizeX >= 0){
			Grid->SNeighbour = &GridArray[i - GridSizeX];
		}
		
	}
}

void AGridManager::SetNeighbourArray() {
	for (int i = 0; i < GridArray.Num(); i++) {
		AGridCell* Grid = GridArray[i];
		Grid->SetNeighbours();
	}
}

void AGridManager::InitGameManager() 
{
	GameManager = GetWorld()->SpawnActor<AGameManager>(GameManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	
	GameManager->GridArray = GridArray;
	GameManager->Init();
}

// A* Pathfinding

TArray<AGridCell *> AGridManager::FindPath(AGridCell *Start, AGridCell *End)
{
	// Create Open and Closed Lists
	TArray<AGridCell*> OpenList;
	TArray<AGridCell*> ClosedList;

	// Add Start to Open List
	OpenList.Add(Start);

	// While Open List is not empty
	while (OpenList.Num() > 0) {
		// Get the lowest F Cost Cell
		AGridCell* CurrentCell = OpenList[0];
		for (int i = 0; i < OpenList.Num(); i++) {
			if (OpenList[i]->FCost() < CurrentCell->FCost() || OpenList[i]->FCost() == CurrentCell->FCost() && OpenList[i]->HCost < CurrentCell->HCost) {
				CurrentCell = OpenList[i];
			}
		}

		// Remove Current Cell from Open List and add to Closed List
		OpenList.Remove(CurrentCell);
		ClosedList.Add(CurrentCell);

		// If Current Cell is the End Cell
		if (CurrentCell == End) {
			return RetracePath(Start, End);
		}

		// Loop through Current Cell's Neighbours
		for (int i = 0; i < CurrentCell->Neighbours.Num(); i++) {
			AGridCell* Neighbour = CurrentCell->Neighbours[i];

			// If Neighbour is not walkable or is in Closed List
			if (!Neighbour->bWalkable || ClosedList.Contains(Neighbour)) {
				continue;
			}

			// Calculate new movement cost to Neighbour
			int NewMovementCostToNeighbour = CurrentCell->GCost + GetDistance(CurrentCell, Neighbour);

			// If new movement cost is lower than Neighbour's G Cost or Neighbour is not in Open List
			if (NewMovementCostToNeighbour < Neighbour->GCost || !OpenList.Contains(Neighbour)) {
				// Set Neighbour's G Cost to new movement cost
				Neighbour->GCost = NewMovementCostToNeighbour;
				// Set Neighbour's H Cost to distance from Neighbour to End
				Neighbour->HCost = GetDistance(Neighbour, End);
				// Set Neighbour's Parent to Current Cell
				Neighbour->Parent = CurrentCell;

				// If Neighbour is not in Open List
				if (!OpenList.Contains(Neighbour)) {
					// Add Neighbour to Open List
					OpenList.Add(Neighbour);
				}
			}
		}
	}
	// If no path is found, return an empty array
	if (OpenList.Num() == 0) {
    	return TArray<AGridCell*>();
	}
	return TArray<AGridCell*>(); // Should never be reached
}

int AGridManager::GetDistance(AGridCell* cellA, AGridCell* cellB) {
	// Manhattan Distance
	int dstX = FMath::Abs(cellA->GetActorLocation().X - cellB->GetActorLocation().X);
	int dstY = FMath::Abs(cellA->GetActorLocation().Y - cellB->GetActorLocation().Y);

	return 10 * (dstX + dstY);
}

TArray<AGridCell*> AGridManager::RetracePath(AGridCell* startCell, AGridCell* endCell) {
    TArray<AGridCell*> path;
    AGridCell* currentCell = endCell;

    while (currentCell != startCell) {
        path.Add(currentCell);
        currentCell = currentCell->Parent;
    }

    Algo::Reverse(path);
	
	// Reset Pathfinding
	for (int i = 0; i < GridArray.Num(); i++) {
		GridArray[i]->ResetPathfinding();
	}
    return path;
}