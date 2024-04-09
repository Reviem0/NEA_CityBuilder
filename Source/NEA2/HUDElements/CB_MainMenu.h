// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "CB_MainMenu.generated.h"
UCLASS()
class NEA2_API UCB_MainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnStartGameButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();


};
