// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Grid.h"
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
	GridArray.Empty();
	//Calculate Offset
	if (AGridBP == nullptr){
		UE_LOG(LogTemp, Warning, TEXT("Grid BP is null"));
		return;
	}

	WorldOffset = GridSize*WorldGridSize/2 - WorldGridSize/2;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int x = 0; x < GridSize; x++) {
		for (int y = 0; y < GridSize; y++) {
			float xLoc = x * WorldGridSize - trunc(WorldOffset);
			float yLoc = y * WorldGridSize - trunc(WorldOffset);
			FVector GridPosition = FVector(xLoc, yLoc, HeightOffset);
			FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(GridPosition, FRotator::ZeroRotator, FVector(objsize,objsize,objsize));
			AGrid* Grid = GetWorld()->SpawnActor<AGrid>(AGridBP, SpawnTransform, SpawnInfo);
			GridArray.Add(Grid);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Grid Generated Count = %d"),GridArray.Num());
}