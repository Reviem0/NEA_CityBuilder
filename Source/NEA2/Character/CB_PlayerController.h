// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Grid/GridManager.h"
#include "../GameObjects/CB_RoadCell.h"
#include "../GameComponents/CB_ClickComponent.h"
#include "../GameComponents/CB_PloppableComponent.h"

#include "CB_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACB_PlayerController();

	UCB_PloppableComponent* NewComponent;

	AGridManager* GridManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Placement")
	TSubclassOf<ACB_BuildingAsset> ActorToPlace;

	ACB_BuildingAsset* PlaceableActor;

	FTransform PlacementTransform;
	bool PlacementModeEnabled = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time")
	float FastForward = 3.f;

	bool bDeletingBuilding;
    FTimerHandle DeleteBuildingTimerHandle;
    float DeleteBuildingInterval = 0.08f;

	AGridCell* LastPlaced;

	int RoadInventory = 0;

	void UpdateRoadInventory(int Amount);

	UFUNCTION()
	void SetPlacementModeEnabled(bool Enabled);
	void PlacementCheck();
	void UpdatePlacement();
	void SpawnBuilding();
	void DeleteBuilding();

    void StartDeletingBuilding();

    void StopDeletingBuilding();
	void PathTEST();

	void IncreaseSpeed();
	void ResetSpeed();
	void UpdateSpeed();

	void ForceLoss();

protected:

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
