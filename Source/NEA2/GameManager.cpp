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

	// Set default values
	TotalScore = 0;
	AvailableColours = {EBuildingClass::Red, EBuildingClass::Blue, EBuildingClass::Green, EBuildingClass::Yellow};

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	// Get grid manager
	AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	if (GridManager) {
		// Get grid array and size
		GridArray = GridManager->PlayGridArray;
		GridSizeX = GridManager->PlayGridSizeX;
		GridSizeY = GridManager->PlayGridSizeY;

		// Initialise the game
		Init();
	}
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Call the score function every frame
	ScoreFunction();
}


void AGameManager::Init()
{
	// Spawn Red Workplace
	SpawnWorkplaceAtRandomLocation(EBuildingClass::Red);
}

bool AGameManager::SpawnHouse(AGridCell* GridCell, EBuildingClass BuildingClass) 
{
	// Check if the grid cell is valid and not occupied
	if (GridCell == nullptr) return false;

	// If the specified grid cell is already occupied, return false
	if (GridCell->OccupyingType != EBuildingType::None) return false;

	// Check if the house classes are valid
	TSubclassOf<ACB_House> HouseClass = nullptr;
	switch (BuildingClass)
	{
	case EBuildingClass::Red: // If specified colour is red, spawn red house
		HouseClass = HouseRedClass;
		break;
		
	case EBuildingClass::Blue: // If specified colour is blue, spawn blue house
		HouseClass = HouseBlueClass;
		break;
		
	case EBuildingClass::Green: // If specified colour is green, spawn green house
		HouseClass = HouseGreenClass;
		break;
		
	case EBuildingClass::Yellow: // If specified colour is yellow, spawn yellow house
		HouseClass = HouseYellowClass;
		break;

	default: // If no colour is specified, return false
		return false;
	}

	// Spawn the house at the specified grid cell location
	ACB_House* House = GetWorld()->SpawnActor<ACB_House>(HouseClass, GridCell->GetActorLocation(), FRotator::ZeroRotator);
	if (House) {
		// Add the house to the house array
		HouseArray.Add(House);
		// Add the workspaces to the house's target workplaces
		TArray<ACB_Workplace*> TargetWorkplaces;
		for (auto& Workplace : WorkplaceArray)
		{
			// Check if the workplace and house are of the same class
			if (Workplace->BuildingClass == House->BuildingClass) {
				TargetWorkplaces.Add(Workplace);
			}
		}
		// Add the target workplaces to the house
		House->AddTargetWorkplace(TargetWorkplaces);
	} else {
		// Failed to spawn
		return false;
	}
	// Spawn successful
	return true;
	
}

bool AGameManager::SpawnWorkplace(AGridCell* GridCell, EBuildingClass BuildingClass) // Default parameter value
{	
	// Check if the grid cell is valid and not occupied
	if (GridCell->OccupyingType != EBuildingType::None) return false;
	// Check if the workplace classes are valid
	if (WorkplaceRedClass == nullptr) return false;
	if (WorkplaceBlueClass == nullptr) return false;
	if (WorkplaceGreenClass == nullptr) return false;
	if (WorkplaceYellowClass == nullptr) return false;

	TSubclassOf<ACB_Workplace> WorkplaceClass = nullptr;
	switch (BuildingClass)
	{
	case EBuildingClass::Red: // If specified colour is red, spawn red workplace
		WorkplaceClass = WorkplaceRedClass;
		break;
	case EBuildingClass::Blue: // If specified colour is blue, spawn blue workplace
		WorkplaceClass = WorkplaceBlueClass;
		break;
	case EBuildingClass::Green: // If specified colour is green, spawn green workplace
		WorkplaceClass = WorkplaceGreenClass;
		break;
	case EBuildingClass::Yellow: // If specified colour is yellow, spawn yellow workplace
		WorkplaceClass = WorkplaceYellowClass;
		break;
	default: // If no colour is specified, return false
		return false;
	}
	ACB_Workplace* Workplace = GetWorld()->SpawnActor<ACB_Workplace>(WorkplaceClass, GridCell->GetActorLocation(), FRotator::ZeroRotator);
	if (Workplace) {
		// Add the workplace to the workplace array
		WorkplaceArray.Add(Workplace);
		// Add the workplace to the house's target workplaces
		for (int i = 0; i < HouseArray.Num(); i++)
		{
			// Check if the house and workplace are of the same class
			if (HouseArray[i]->BuildingClass == Workplace->BuildingClass) {
				HouseArray[i]->AddTargetWorkplace(Workplace);
			}
		}
	} else {
		// Failed to spawn
		return false;
	}
	// Check if the houses need to be spawned
	SatisfactionCheck();
	// Spawn successful
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
	// Attempt to spawn until successful or limit reached
	while (!SpawnSuccess && SpawnAttemptCount < SpawnAttemptLimit) {
		SpawnAttemptCount++;
		int Attempts = 0;
		// Find a valid grid cell to spawn the house on three attempts
		while (!SpawnCell && Attempts < 3) {
			Attempts++;
			int RandomIndex = FMath::RandRange(0, GridArray.Num()-1);
			if (GridArray[RandomIndex]->OccupyingType == EBuildingType::None) {
				SpawnCell = GridArray[RandomIndex];
			}
		}
		// If a valid grid cell is found, attempt to spawn the house
		if (SpawnCell){
			SpawnSuccess = SpawnHouse(SpawnCell, BuildingClass);
		}
	}
	// if the house is successfully spawned, return true
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

	TArray<AGridCell*> PossibleSpawnLocations;
	// Create a list of possible spawn locations
	for (int i = 0; i < GridArray.Num(); i++) {
		if (GridArray[i]->OccupyingType == EBuildingType::None) {
			// Ensure that the workplace is not spawned on the edge of the grid
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
		// Workplace is 2x2 so it needs to be 2 away from the edge
		int RandomNumberX = FMath::RandRange(0, PossibleSpawnLocations.Num() - 1);
		SpawnSuccess = SpawnWorkplace(PossibleSpawnLocations[RandomNumberX], BuildingClass);
		if (SpawnSuccess) {
			UE_LOG(LogTemp, Display, TEXT("WORKPLACE SPAWN SUCCESSFUL"));
			break;
		}
	}
	// if the workplace is successfully spawned, return true
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
	// Update the path for all houses
	for (int i = 0; i < HouseArray.Num(); i++) {
		HouseArray[i]->CreatePath();
		UE_LOG(LogTemp, Display, TEXT("UPDATING PATH FOR HOUSE: %s"), *HouseArray[i]->GetName());
	}
}

void AGameManager::SpawnColourSet(EBuildingClass BuildingClass) { // Redundant function

	// If no colour is specified, spawn a random colour
	if (BuildingClass == EBuildingClass::None) {
		BuildingClass = AvailableColours[FMath::RandRange(0, AvailableColours.Num() - 1)];
	}
	
	// Spawn a random colour set from the available colours
	if (SpawnWorkplaceAtRandomLocation(BuildingClass)){
		// SpawnHouseAtRandomLocation(BuildingClass);
	}
}

void AGameManager::AddScore(int Score) 
{
	// Add the score to the total score
	TotalScore += Score;
	// Log the total score
	UE_LOG(LogTemp, Display, TEXT("TOTAL SCORE: %d"), TotalScore);
	// Call the score function
	// ScoreFunction();
	
}

void AGameManager::ScoreFunction() {
	//Get player controller
	ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// Get grid manager
	AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));

	// Get GameMode
	ANEA2GameModeBase* GameMode = Cast<ANEA2GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	// Spawn a workplace every 60 seconds
	if (GameMode->time % 60 == 0 && GameMode->time != LastTime) {
		SpawnWorkplaceAtRandomLocation();
	}
	// Update the road inventory every 160 seconds
	if (GameMode->time % 160 == 0 && GameMode->time != LastTime) {
		PlayerController->UpdateRoadInventory(20);

	}
	// Expand the grid every 200 seconds
	if (GameMode->time % 200 == 0 && GameMode->time != LastTime) {
		GridManager->ExpandSubGrid(2,2);
		SpawnWorkplaceAtRandomLocation();
	}
	LastTime = GameMode->time;
} 

/* void AGameManager::ScoreFunction() {
	//Get player controller
	ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// Get grid manager
	AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	if (TotalScore % 5 == 0) {
		SpawnWorkplaceAtRandomLocation();
	}
	if (TotalScore % 10 == 0) {
		GridManager->ExpandSubGrid(2,2);
		PlayerController->UpdateRoadInventory(20);
		UE_LOG(LogTemp, Display, TEXT("New Road Inventory: %d"), PlayerController->RoadInventory);
	}
} */

void AGameManager::SatisfactionCheck() 
{
	// Initialise the total goal for each colour
	int redGoal = 0;
	int blueGoal = 0;
	int greenGoal = 0;
	int yellowGoal = 0;

	// Initialise the total house count for each colour
	int redHouseCount = 0;
	int blueHouseCount = 0;
	int greenHouseCount = 0;
	int yellowHouseCount = 0;

	// Calculate the total goal and house count for each colour
	for (ACB_Workplace* Workplace : WorkplaceArray) {
		switch (Workplace->BuildingClass) {
			case EBuildingClass::Red:    // If the workplace is red, add the goal to the red goal
				redGoal += Workplace->Goal;
				break;
			case EBuildingClass::Blue:   // If the workplace is blue, add the goal to the blue goal
				blueGoal += Workplace->Goal;
				break;
			case EBuildingClass::Green:  // If the workplace is green, add the goal to the green goal
				greenGoal += Workplace->Goal;
				break;
			case EBuildingClass::Yellow: // If the workplace is yellow, add the goal to the yellow goal
				yellowGoal += Workplace->Goal;
				break;
			default: // If the workplace is not of a valid colour, skip
				break;
		}
	}
	
	for (ACB_House* House : HouseArray) {
		switch (House->BuildingClass) {
			case EBuildingClass::Red:   // If the house is red, add to the red house count
				redHouseCount += 1;
				break;
			case EBuildingClass::Blue:  // If the house is blue, add to the blue house count
				blueHouseCount += 1;
				break;
			case EBuildingClass::Green: // If the house is green, add to the green house count
				greenHouseCount += 1;
				break;
			case EBuildingClass::Yellow:// If the house is yellow, add to the yellow house count
				yellowHouseCount += 1;
				break;
			default: // If the house is not of a valid colour, skip
				break;
		}
	}

	// Spawn red houses if the goal cannot be met with the current number of red houses
	if (redGoal / 20.0f > redHouseCount) {
		for (int i = 0; i < ceil(redGoal / 20.0f) - redHouseCount; i++) {
			UE_LOG(LogTemp, Display, TEXT("SATISFACTION SPAWNING RED HOUSE"));
			SpawnHouseAtRandomLocation(EBuildingClass::Red);
		}
	}
	
	// Spawn blue houses if the goal cannot be met with the current number of blue houses
	if (blueGoal / 20.0f > blueHouseCount) {
		for (int i = 0; i < ceil(blueGoal / 20.0f) - blueHouseCount; i++) {
			UE_LOG(LogTemp, Display, TEXT("SATISFACTION SPAWNING BLUE HOUSE"));
			SpawnHouseAtRandomLocation(EBuildingClass::Blue);
		}
	}
	
	// Spawn green houses if the goal cannot be met with the current number of green houses
	if (greenGoal / 20.0f > greenHouseCount) {
		for (int i = 0; i < ceil(greenGoal / 20.0f) - greenHouseCount; i++) {
			UE_LOG(LogTemp, Display, TEXT("SATISFACTION SPAWNING GREEN HOUSE"));
			SpawnHouseAtRandomLocation(EBuildingClass::Green);
		}
	}
	
	// Spawn yellow houses if the goal cannot be met with the current number of yellow houses
	if (yellowGoal / 20.0f > yellowHouseCount) {
		for (int i = 0; i < ceil(yellowGoal / 20.0f) - yellowHouseCount; i++) {
			UE_LOG(LogTemp, Display, TEXT("SATISFACTION SPAWNING YELLOW HOUSE"));
			SpawnHouseAtRandomLocation(EBuildingClass::Yellow);
		}
	}
}

int AGameManager::GetScore() 
{
	return TotalScore;
}

void AGameManager::GameOver() 
{
	// Check if the game has already been lost
	if (hasLost) return;

	// Set the game to lost
	hasLost = true;
	// Get player controller
	ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Disable player input and show the mouse cursor
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;

	// Get the loss screen class
	if (LossScreenClass) {
		// Create the loss screen widget and add it to the viewport
		UUserWidget* LossScreen = CreateWidget<UUserWidget>(GetWorld(), LossScreenClass);
		if (LossScreen) {
			LossScreen->AddToViewport();
		}
	}

}
