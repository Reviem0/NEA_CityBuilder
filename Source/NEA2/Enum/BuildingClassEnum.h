// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "BuildingClassEnum.generated.h"


UENUM(BlueprintType)
enum class EBuildingClass :uint8
{
	Red   = 0 UMETA(DisplayName = "Red"),
	Green = 1 UMETA(DisplayName = "Green"),
	Blue   = 2 UMETA(DisplayName = "Blue"),
	Yellow  = 3 UMETA(DisplayName = "Yellow")
};
UCLASS()
class NEA2_API UBuildingClassEnum : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
