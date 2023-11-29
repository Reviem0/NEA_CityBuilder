// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CB_BuildingAsset.h"
#include "../../Enum/BuildingClassEnum.h"

#include "CB_House.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_House : public ACB_BuildingAsset
{
	GENERATED_BODY()
public:
	ACB_House();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	EBuildingClass BuildingClass;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	

	
};
