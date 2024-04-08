// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HUDElements/CB_MainMenu.h"

#include "CB_MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_MainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	void LoadMainMenu();
	
};
