// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "OwnershipEnum.generated.h"

UENUM(BlueprintType)
enum class EOwnership :uint8
{
	None      = 0 UMETA(DisplayName = "None"),
	Player    = 1 UMETA(DisplayName = "Player"),
	Game 	  = 2 UMETA(DisplayName = "Game"),

};

UCLASS()
class NEA2_API UOwnershipEnum : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
