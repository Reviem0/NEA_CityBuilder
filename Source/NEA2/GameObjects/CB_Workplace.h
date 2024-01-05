// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_BuildingAsset.h"
#include "Workplace/CB_WorkplaceAsset.h"
#include "../Enum/BuildingClassEnum.h"
#include "CB_OwnedRoadCell.h"
#include "CB_Workplace.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_Workplace : public ACB_BuildingAsset
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACB_Workplace();

	EBuildingClass BuildingClass;

	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_WorkplaceAsset> TopRightActor;
	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_WorkplaceAsset> TopLeftActor;
	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_WorkplaceAsset> BottomRightActor;
	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_WorkplaceAsset> BottomLeftActor;
	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_OwnedRoadCell> RoadTileActor;

	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_WorkplaceAsset* TopRightAsset;
	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_WorkplaceAsset* TopLeftAsset;
	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_WorkplaceAsset* BottomRightAsset;
	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_WorkplaceAsset* BottomLeftAsset;
	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_OwnedRoadCell* RoadTileAsset;

	

	



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
