// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Grid/GridManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	if (GridManager) {
		GridArray = GridManager->GridArray;
		GridSizeX = GridManager->GridSizeX;
		GridSizeY = GridManager->GridSizeY;

		Init();
	}


	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGameManager::Init()
{
	SpawnHouseAtRandomLocation();
	SpawnWorkplaceAtRandomLocation();
}

bool AGameManager::SpawnHouse(AGridCell* GridCell) 
{
	if (HouseClass == nullptr) return false;

	ACB_House* House = GetWorld()->SpawnActor<ACB_House>(HouseClass, GridCell->GetActorLocation(), FRotator::ZeroRotator);
	if (House) {
		HouseArray.Add(House);
	} else {
		// Failed to spawn
		return false;
	}
	// Spawn successful
	return true;
	
}

void AGameManager::SpawnHouseAtRandomLocation() 
{
	// Ensure House is not spawned on another object
	AGridCell* SpawnCell = nullptr;
	int SpawnAttemptCount = 0;
	bool SpawnSuccess = false;
	while (!SpawnSuccess && SpawnAttemptCount < SpawnAttemptLimit) {
		while (!SpawnCell) {
			int RandomIndex = FMath::RandRange(0, GridArray.Num()-1);
			if (GridArray[RandomIndex]->OccupyingType == EBuildingType::None) {
				SpawnCell = GridArray[RandomIndex];
			}
		}
		SpawnAttemptCount++;
		SpawnSuccess = SpawnHouse(SpawnCell);
	}
	if (SpawnSuccess) {
		UE_LOG(LogTemp, Display, TEXT("HOUSE SPAWN SUCCESSFUL"));
	} else {
		UE_LOG(LogTemp, Display, TEXT("HOUSE SPAWN FAILED"));
	}
}

bool AGameManager::SpawnWorkplace(AGridCell* GridCell)
{
	if (WorkplaceClass == nullptr) return false;

	ACB_Workplace* Workplace = GetWorld()->SpawnActor<ACB_Workplace>(WorkplaceClass, GridCell->GetActorLocation(), FRotator::ZeroRotator);
	if (Workplace) {
		WorkplaceArray.Add(Workplace);
		for (int i = 0; i < HouseArray.Num(); i++)
		{
			HouseArray[i]->TargetWorkplaces.Add(Workplace);
		}
	} else {
		// Failed to spawn
		return false;
	}
	// Spawn successful
	return true;
}

void AGameManager::SpawnWorkplaceAtRandomLocation() 
{
	if (WorkplaceClass == nullptr) return;
	bool SpawnSuccess = false;
	int SpawnAttemptCount = 0;
	// Attempt to spawn until successful
	while (!SpawnSuccess && SpawnAttemptCount < SpawnAttemptLimit) 
	{
		// Ensure that the workplace is not spawned on the edge of the grid
		// Workplace is 2x2 so it needs to be 2 away from the edge
		int RandomNumberX = FMath::RandRange(0, GridSizeX - 2);
		int RandomNumberY = FMath::RandRange(0, GridSizeY - 2);
		int index = RandomNumberY * GridSizeX + RandomNumberX;

		UE_LOG(LogTemp, Display, TEXT("WORKPLACE: index: %d"), index);
		SpawnAttemptCount++;
		SpawnSuccess = SpawnWorkplace(GridArray[index]);
	}
	if (SpawnSuccess) {
		UE_LOG(LogTemp, Display, TEXT("WORKPLACE SPAWN SUCCESSFUL"));
	} else {
		UE_LOG(LogTemp, Display, TEXT("WORKPLACE SPAWN FAILED"));
	}
    
}

void AGameManager::UpdatePath() 
{
	for (int i = 0; i < HouseArray.Num(); i++) {
		HouseArray[i]->CreatePath();
		UE_LOG(LogTemp, Display, TEXT("UPDATING PATH FOR HOUSE: %s"), *HouseArray[i]->GetName());
	}
}
