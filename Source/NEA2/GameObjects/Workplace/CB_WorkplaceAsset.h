// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CB_BuildingAsset.h"
#include "CB_WorkplaceAsset.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_WorkplaceAsset : public ACB_BuildingAsset
{
	GENERATED_BODY()

public:
	ACB_WorkplaceAsset();
	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_BuildingAsset* WorkplaceRef;


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};
