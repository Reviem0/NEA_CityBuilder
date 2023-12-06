// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_RoadTile.h"
#include "CB_OwnedRoadTile.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_OwnedRoadTile : public ACB_RoadTile
{
	GENERATED_BODY()

public:
	ACB_OwnedRoadTile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	
};
