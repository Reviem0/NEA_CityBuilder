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
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridManager::PopulateGrid() {
	FTransform Transform;
	GridArray.Empty(); 
	FVector GridPos = FVector::ZeroVector;

	FVector GridScale = FVector(WorldGridSize/100, WorldGridSize/100, WorldGridSize/100);
	GridPos.Z = HeightOffset;
	
	
	int GridSize = GridSizeX * GridSizeY;
	
	// Calculate the center offset
	float CenterOffsetX = (GridSizeX * WorldGridSize) / 2.0f - WorldGridSize / 2.0f; 
	float CenterOffsetY = (GridSizeY * WorldGridSize) / 2.0f - WorldGridSize / 2.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < GridSizeY; i++) {
		for (int j = 0; j < GridSizeX; j++) {
			GridPos.X = (j * WorldGridSize) - CenterOffsetX; 
			GridPos.Y = (i * WorldGridSize) - CenterOffsetY;
			
			Transform = UKismetMathLibrary::MakeTransform(GridPos, FRotator::ZeroRotator, GridScale);
			AGridCell* Grid = GetWorld()->SpawnActor<AGridCell>(GridCell, Transform, SpawnParams);
			Grid->SetActorRelativeScale3D(GridScale);
			GridArray.Add(Grid);
		}
	}
}

FVector AGridManager::GetClosestGridPosition(FVector InPosition)
{
	FVector ClosestPosition;
	ClosestPosition = GridArray[0]->GetActorLocation();
	float ClosestDistance = FVector::Dist(InPosition, ClosestPosition);

	for (int i = 0; i < GridArray.Num(); i++) {
		AGridCell* Grid = GridArray[i];
		if (FVector::Dist(InPosition, Grid->GetActorLocation()) < ClosestDistance) {
			ClosestPosition = Grid->GetActorLocation();
			ClosestDistance = FVector::Dist(InPosition, ClosestPosition);
		}
	}

    return ClosestPosition;
}
