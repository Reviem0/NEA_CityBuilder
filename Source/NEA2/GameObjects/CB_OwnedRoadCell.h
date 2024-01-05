// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_RoadCell.h"
#include "CB_OwnedRoadCell.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_OwnedRoadCell : public ACB_RoadCell
{
	GENERATED_BODY()

public:
	ACB_OwnedRoadCell();
	AGridCell* OwningCell;

	void UpdateRoadMesh() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	
};
