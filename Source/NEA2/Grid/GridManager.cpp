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
	float CenterOffsetX = ((GridSizeX-1) * WorldGridSize) / 2.0f; 
	float CenterOffsetY = ((GridSizeY-1) * WorldGridSize) / 2.0f;

	//Grid Spawn Parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform Transform;
	FVector GridPos = FVector::ZeroVector;
	FVector GridScale = GetGridScale();
	GridPos.Z = HeightOffset;

	// Spawn Grid
	for (int i = 0; i < GridSizeX; i++) {
		for (int j = 0; j < GridSizeY; j++) {
			// Calculate Spawn Point of Grid Cell
			GridPos.X = (j * WorldGridSize) - CenterOffsetX; 
			GridPos.Y = (i * WorldGridSize) - CenterOffsetY;
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
	int ClosestIndex = 0;
	float ClosestDistance = FVector::Dist(InPosition, GridArray[ClosestIndex]->GetActorLocation());
	// Search for closest Grid Cell to inPosition
	for (int i = 0; i < GridArray.Num(); i++) {
		AGridCell* Grid = GridArray[i];
		if (FVector::Dist(InPosition, Grid->GetActorLocation()) < ClosestDistance) {
			ClosestIndex = i;
			ClosestDistance = FVector::Dist(InPosition, GridArray[ClosestIndex]->GetActorLocation());
		}
	}

    return GridArray[ClosestIndex];
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

		// Get North Neighbour
		if (i + GridSizeX < GridArray.Num()) {
			Grid->NNeighbour = &GridArray[i + GridSizeX];
		}

		// Get South Neighbour
		if (i - GridSizeX >= 0){
			Grid->SNeighbour = &GridArray[i - GridSizeX];
		}

		// Get West Neighbour
		if ((i+1) % GridSizeX != 0 ) {
			Grid->WNeighbour = &(GridArray[i + 1]);
		}

		// Get East Neighbour
		if (i % GridSizeX != 0 && i != 0) {
			 Grid->ENeighbour = &(GridArray[i - 1]);
		}
		
	}
}
