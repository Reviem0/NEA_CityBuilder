// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HUDElements/CB_PlayerHUD.h"

#include "NEA2GameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class NEA2_API ANEA2GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANEA2GameModeBase();
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> PlayerHUDClass;

	UCB_PlayerHUD* PlayerHUD;

	void UpdateTime();
	int time = 0;
	
};
