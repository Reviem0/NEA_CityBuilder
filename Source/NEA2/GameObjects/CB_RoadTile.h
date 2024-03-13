// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CB_BuildingAsset.h"

#include "CB_RoadTile.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_RoadTile : public ACB_BuildingAsset
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACB_RoadTile();

	UPROPERTY(VisibleAnywhere, Category = "RoadOwner")
	AActor* RoadCellOwner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
