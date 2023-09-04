// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Grid/GridManager.h"
#include "../GameObjects/CB_BuildingAsset.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Placement")
	TSubclassOf<ACB_BuildingAsset> ActorToPlace;

	ACB_BuildingAsset* PlaceableActor;

	FTransform PlacementTransform;
	bool PlacementModeEnabled = false;

	UFUNCTION()
	void SetPlacementModeEnabled(bool Enabled);
	void UpdatePlacement();
	void SpawnBuilding();


protected:

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
