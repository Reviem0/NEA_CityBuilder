// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CB_BuildingAsset.h"
#include "WorkTopMid.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API AWorkTopMid : public ACB_BuildingAsset
{
	GENERATED_BODY()

public:
	AWorkTopMid();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};
