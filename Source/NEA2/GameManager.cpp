// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Grid/GridManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TotalScore = 0;

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

	RemainingColours = {EBuildingClass::Red, EBuildingClass::Blue, EBuildingClass::Green, EBuildingClass::Yellow};
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGameManager::Init()
{
	AvailableColours.Add(EBuildingClass::Red);
	SpawnColourSet();
}

bool AGameManager::SpawnHouse(AGridCell* GridCell, EBuildingClass BuildingClass) 
{
	if (GridCell->OccupyingType != EBuildingType::None) return false;
	TSubclassOf<ACB_House> HouseClass = nullptr;
	if (BuildingClass == EBuildingClass::None) return false;
	if (BuildingClass == EBuildingClass::Red) {
		HouseClass = HouseRedClass;
	} else if (BuildingClass == EBuildingClass::Blue) {
		HouseClass = HouseBlueClass;
	} else if (BuildingClass == EBuildingClass::Green) {
		HouseClass = HouseGreenClass;
	} else if (BuildingClass == EBuildingClass::Yellow) {
		HouseClass = HouseYellowClass;
	} else {
		return false;
	}

	ACB_House* House = GetWorld()->SpawnActor<ACB_House>(HouseClass, GridCell->GetActorLocation(), FRotator::ZeroRotator);
	if (House) {
		HouseArray.Add(House);
		for (auto& Workplace : WorkplaceArray)
		{
			if (Workplace->BuildingClass == House->BuildingClass) {
				House->TargetWorkplaces.Add(Workplace);
				UpdatePath();
			}
		}
	} else {
		// Failed to spawn
		return false;
	}
	// Spawn successful
	return true;
	
}

bool AGameManager::SpawnWorkplace(AGridCell* GridCell, EBuildingClass BuildingClass) // Default parameter value
{	
	if (GridCell->OccupyingType != EBuildingType::None) return false;
	if (WorkplaceRedClass == nullptr) return false;
	if (WorkplaceBlueClass == nullptr) return false;
	if (WorkplaceGreenClass == nullptr) return false;
	if (WorkplaceYellowClass == nullptr) return false;

	TSubclassOf<ACB_Workplace> WorkplaceClass = nullptr;
	if (BuildingClass == EBuildingClass::None) return false;
	if (BuildingClass == EBuildingClass::Red) {
		WorkplaceClass = WorkplaceRedClass;
	} else if (BuildingClass == EBuildingClass::Blue) {
		WorkplaceClass = WorkplaceBlueClass;
	} else if (BuildingClass == EBuildingClass::Green) {
		WorkplaceClass = WorkplaceGreenClass;
	} else if (BuildingClass == EBuildingClass::Yellow) {
		WorkplaceClass = WorkplaceYellowClass;
	} else {
		return false;
	}

	ACB_Workplace* Workplace = GetWorld()->SpawnActor<ACB_Workplace>(WorkplaceClass, GridCell->GetActorLocation(), FRotator::ZeroRotator);
	if (Workplace) {
		WorkplaceArray.Add(Workplace);
		for (int i = 0; i < HouseArray.Num(); i++)
		{
			HouseArray[i]->TargetWorkplaces.Add(Workplace);
			UpdatePath();
		}
	} else {
		// Failed to spawn
		return false;
	}
	// Spawn successful
	return true;
}

void AGameManager::SpawnHouseAtRandomLocation(EBuildingClass BuildingClass) 
{
	// Ensure House is not spawned on another object
	AGridCell* SpawnCell = nullptr;

	// If no colour is specified, spawn a random colour
	if (BuildingClass == EBuildingClass::None) {
		BuildingClass = AvailableColours[FMath::RandRange(0, AvailableColours.Num() - 1)];
	}

	int SpawnAttemptCount = 0;
	bool SpawnSuccess = false;
	while (!SpawnSuccess && SpawnAttemptCount < SpawnAttemptLimit) {
		while (!SpawnCell) {
			int RandomIndex = FMath::RandRange(0, GridArray.Num()-1);
			if (GridArray[RandomIndex]->OccupyingType == EBuildingType::None) {
				SpawnCell = GridArray[RandomIndex];
			}
		}
		SpawnAttemptCount += 1;
		SpawnSuccess = SpawnHouse(SpawnCell, BuildingClass);
	}
	if (SpawnSuccess) {
		UE_LOG(LogTemp, Display, TEXT("HOUSE SPAWN SUCCESSFUL"));
	} else {
		UE_LOG(LogTemp, Display, TEXT("HOUSE SPAWN FAILED"));
	}
}

void AGameManager::SpawnWorkplaceAtRandomLocation(EBuildingClass BuildingClass) 
{
	if (WorkplaceRedClass == nullptr) return;

	// If no colour is specified, spawn a random colour
	if (BuildingClass == EBuildingClass::None) {
		BuildingClass = AvailableColours[FMath::RandRange(0, AvailableColours.Num() - 1)];
	}

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
		SpawnAttemptCount += 1;
		SpawnSuccess = SpawnWorkplace(GridArray[index], BuildingClass);
	}
	if (SpawnSuccess) {
		UE_LOG(LogTemp, Display, TEXT("WORKPLACE SPAWN SUCCESSFUL"));
		return;
	} else {
		UE_LOG(LogTemp, Display, TEXT("WORKPLACE SPAWN FAILED"));
		return;
	}
    
}

void AGameManager::UpdatePath() 
{
	for (int i = 0; i < HouseArray.Num(); i++) {
		HouseArray[i]->CreatePath();
		UE_LOG(LogTemp, Display, TEXT("UPDATING PATH FOR HOUSE: %s"), *HouseArray[i]->GetName());
	}
}

void AGameManager::SpawnColourSet(EBuildingClass BuildingClass) {

	// If no colour is specified, spawn a random colour
	if (BuildingClass == EBuildingClass::None) {
		BuildingClass = AvailableColours[FMath::RandRange(0, AvailableColours.Num() - 1)];
	}
	
	// Spawn a random colour set from the available colours
	SpawnHouseAtRandomLocation(BuildingClass);
	SpawnWorkplaceAtRandomLocation(BuildingClass);
	

}

void AGameManager::AddScore(int Score) 
{
	TotalScore += Score;
	UE_LOG(LogTemp, Display, TEXT("TOTAL SCORE: %d"), TotalScore);
	ScoreFunction();
	
}

void AGameManager::ScoreFunction() {
	if (TotalScore % 10 == 0 && RemainingColours.Num() > 0) {
		EBuildingClass random = RemainingColours[FMath::RandRange(0, RemainingColours.Num() - 1)];
		AvailableColours.Add(random);
		RemainingColours.Remove(random);
		SpawnColourSet();
	}
	if (TotalScore % 5 == 0) {
		SpawnHouseAtRandomLocation();
	}
	if (TotalScore % 20 == 0) {
		SpawnWorkplaceAtRandomLocation();
	}
}

/* 
Calculating the total score inside the GameManager.cpp: 
This approach is more straightforward and easier to implement. You would simply iterate over all the workplaces and sum their scores. 
However, this could be inefficient if there are a large number of workplaces and the score needs to be updated frequently, as you would be recalculating the 
entire sum every time.

Having the workplaces update the GameManager's total score: This approach is more efficient, as it only requires a single addition operation 
whenever a workplace's score increases. However, it's more complex to implement, as you need to ensure that every workplace has a reference to 
the GameManager and calls the appropriate method whenever its score changes. It also introduces tighter coupling between the workplaces and the
 GameManager, which could make the code harder to maintain and reason about.
  */