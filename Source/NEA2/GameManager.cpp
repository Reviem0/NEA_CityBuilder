// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Grid/GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CB_PlayerController.h"
#include "GameComponents/CB_PloppableComponent.h"
#include "GameFramework/HUD.h"
#include "NEA2GameModeBase.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TotalScore = 0;
	AvailableColours = {EBuildingClass::Red, EBuildingClass::Blue, EBuildingClass::Green, EBuildingClass::Yellow};

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	if (GridManager) {
		GridArray = GridManager->PlayGridArray;
		GridSizeX = GridManager->PlayGridSizeX;
		GridSizeY = GridManager->PlayGridSizeY;

		Init();
	}
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ScoreFunction();
}


void AGameManager::Init()
{
	SpawnWorkplaceAtRandomLocation(EBuildingClass::Red);
}

bool AGameManager::SpawnHouse(AGridCell* GridCell, EBuildingClass BuildingClass) 
{
	if (GridCell == nullptr) return false;
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
			if (HouseArray[i]->BuildingClass == Workplace->BuildingClass) {
				HouseArray[i]->AddTargetWorkplace(Workplace);
			}
		}
	} else {
		// Failed to spawn
		return false;
	}
	// Spawn successful
	SatisfactionCheck();
	return true;
}

bool AGameManager::SpawnHouseAtRandomLocation(EBuildingClass BuildingClass) 
{
	UE_LOG(LogTemp, Display, TEXT("SPAWNING HOUSE"));
	// Ensure House is not spawned on another object
	AGridCell* SpawnCell = nullptr;

	// If no colour is specified, spawn a random colour
	if (BuildingClass == EBuildingClass::None) {
		BuildingClass = AvailableColours[FMath::RandRange(0, AvailableColours.Num() - 1)];
	}

	int SpawnAttemptCount = 0;
	bool SpawnSuccess = false;
	while (!SpawnSuccess && SpawnAttemptCount < SpawnAttemptLimit) {
		SpawnAttemptCount++;
		int Attempts = 0;
		while (!SpawnCell && Attempts < 3) {
			Attempts++;
			int RandomIndex = FMath::RandRange(0, GridArray.Num()-1);
			if (GridArray[RandomIndex]->OccupyingType == EBuildingType::None) {
				SpawnCell = GridArray[RandomIndex];
			}
		}
		SpawnSuccess = SpawnHouse(SpawnCell, BuildingClass);
	}
	if (SpawnSuccess) {
		UE_LOG(LogTemp, Display, TEXT("HOUSE SPAWN SUCCESSFUL"));
		return true;
	} else {
		UE_LOG(LogTemp, Display, TEXT("HOUSE SPAWN FAILED"));
		return false;
	}
	return false;
}

bool AGameManager::SpawnWorkplaceAtRandomLocation(EBuildingClass BuildingClass) 
{
	UE_LOG(LogTemp, Display, TEXT("SPAWNING WORKPLACE"));
	if (WorkplaceRedClass == nullptr) return false;
	
	TArray<AGridCell*> PossibleSpawnLocations;
	// Workplace is 2x2
	// Ensure Workplace is not spawned on another object
	for (int i = 0; i < GridArray.Num(); i++) {
		if (GridArray[i]->OccupyingType == EBuildingType::None) {
			if (i % GridSizeX < GridSizeX - 1 && i / GridSizeX < GridSizeY - 1) {
				if (GridArray[i + 1]->OccupyingType == EBuildingType::None && GridArray[i + GridSizeX]->OccupyingType == EBuildingType::None && GridArray[i + GridSizeX + 1]->OccupyingType == EBuildingType::None) {
					PossibleSpawnLocations.Add(GridArray[i]);
				}
			}
		}
	}

	
	// If no colour is specified, spawn a random colour
	if (BuildingClass == EBuildingClass::None) {
		BuildingClass = AvailableColours[FMath::RandRange(0, AvailableColours.Num() - 1)];
	}

	bool SpawnSuccess = false;
	// Attempt to spawn until successful
	if (PossibleSpawnLocations.Num() == 0) return false;

	for (int i = 0; i < 3; i++)
	{
		// Ensure that the workplace is not spawned on the edge of the grid
		// Workplace is 2x2 so it needs to be 2 away from the edge
		int RandomNumberX = FMath::RandRange(0, PossibleSpawnLocations.Num() - 1);
		SpawnSuccess = SpawnWorkplace(PossibleSpawnLocations[RandomNumberX], BuildingClass);
		if (SpawnSuccess) {
			UE_LOG(LogTemp, Display, TEXT("WORKPLACE SPAWN SUCCESSFUL"));
			break;
		}
	}
	if (SpawnSuccess) {
		return true;
	} else {
		UE_LOG(LogTemp, Display, TEXT("WORKPLACE SPAWN FAILED"));
		return false;
	}
	return false;
    
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
	if (SpawnWorkplaceAtRandomLocation(BuildingClass)){
		//SpawnHouseAtRandomLocation(BuildingClass);
	}
}

void AGameManager::AddScore(int Score) 
{
	TotalScore += Score;
	UE_LOG(LogTemp, Display, TEXT("TOTAL SCORE: %d"), TotalScore);
	
}

void AGameManager::ScoreFunction() {
	//Get player controller
	ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// Get grid manager
	AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));

	// Get GameMode
	ANEA2GameModeBase* GameMode = Cast<ANEA2GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode->time % 60 == 0 && GameMode->time != LastTime) {
		SpawnWorkplaceAtRandomLocation();
	}
	if (GameMode->time % 160 == 0 && GameMode->time != LastTime) {
		PlayerController->UpdateRoadInventory(20);

	}
	if (GameMode->time % 200 == 0 && GameMode->time != LastTime) {
		GridManager->ExpandSubGrid(2,2);
		SpawnWorkplaceAtRandomLocation();
	}
	LastTime = GameMode->time;
}

void AGameManager::SatisfactionCheck() 
{
	int redGoal = 0;
	int blueGoal = 0;
	int greenGoal = 0;
	int yellowGoal = 0;

	int redHouseCount = 0;
	int blueHouseCount = 0;
	int greenHouseCount = 0;
	int yellowHouseCount = 0;

	for (ACB_Workplace* Workplace : WorkplaceArray) {
		if (Workplace->BuildingClass == EBuildingClass::Red) {
			redGoal += Workplace->Goal;
		} else if (Workplace->BuildingClass == EBuildingClass::Blue) {
			blueGoal += Workplace->Goal;
		} else if (Workplace->BuildingClass == EBuildingClass::Green) {
			greenGoal += Workplace->Goal;
		} else if (Workplace->BuildingClass == EBuildingClass::Yellow) {
			yellowGoal += Workplace->Goal;
		}
	}
	
	for (ACB_House* House : HouseArray) {
		if (House->BuildingClass == EBuildingClass::Red) {
			redHouseCount += 1;
		} else if (House->BuildingClass == EBuildingClass::Blue) {
			blueHouseCount += 1;
		} else if (House->BuildingClass == EBuildingClass::Green) {
			greenHouseCount += 1;
		} else if (House->BuildingClass == EBuildingClass::Yellow) {
			yellowHouseCount += 1;
		}
	}

	if (true) {
		if (redGoal / 20 > redHouseCount) {
			for (int i = 0; i < redGoal / 20 - redHouseCount; i++) {
				UE_LOG(LogTemp, Display, TEXT("SATISFACTION SPAWNING RED HOUSE"));
				SpawnHouseAtRandomLocation(EBuildingClass::Red);
			}
		}
	}
	if (true) {
		if (blueGoal / 20 > blueHouseCount) {
			for (int i = 0; i < blueGoal / 20 - blueHouseCount; i++) {
				UE_LOG(LogTemp, Display, TEXT("SATISFACTION SPAWNING BLUE HOUSE"));
				SpawnHouseAtRandomLocation(EBuildingClass::Blue);
			}
		}
	}
	if (true) {
		if (greenGoal / 20 > greenHouseCount) {
			for (int i = 0; i < greenGoal / 20 - greenHouseCount; i++) {
				UE_LOG(LogTemp, Display, TEXT("SATISFACTION SPAWNING GREEN HOUSE"));
				SpawnHouseAtRandomLocation(EBuildingClass::Green);
			}
		}
	}
	if (true) {
		if (yellowGoal / 20 > yellowHouseCount) {
			for (int i = 0; i < yellowGoal / 20 - yellowHouseCount; i++) {
				UE_LOG(LogTemp, Display, TEXT("SATISFACTION SPAWNING YELLOW HOUSE"));
				SpawnHouseAtRandomLocation(EBuildingClass::Yellow);
			}
		}
	}
}

int AGameManager::GetScore() 
{
	return TotalScore;
}

void AGameManager::LossFunction() 
{
	if (hasLost) return;
	hasLost = true;
	// Get player controlle
	ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;

	if (LossScreenClass) {
		UUserWidget* LossScreen = CreateWidget<UUserWidget>(GetWorld(), LossScreenClass);
		if (LossScreen) {
			LossScreen->AddToViewport();
		}
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