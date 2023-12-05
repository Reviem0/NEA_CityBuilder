// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_BuildingAsset.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
