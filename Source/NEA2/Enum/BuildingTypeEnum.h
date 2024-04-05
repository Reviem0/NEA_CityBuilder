// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "BuildingTypeEnum.generated.h"

UENUM(BlueprintType)
enum class EBuildingType :uint8
{
	None      = 0 UMETA(DisplayName = "None"),
	Placed    = 1 UMETA(DisplayName = "Placed"),
	Road      = 2 UMETA(DisplayName = "Road"),
	OwnedRoad = 3 UMETA(DisplayName = "OwnedRoad"),
	House  	  = 4 UMETA(DisplayName = "House"),
	Workplace = 5 UMETA(DisplayName = "Workplace")
};

UCLASS()
class NEA2_API UBuildingTypeEnum : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
