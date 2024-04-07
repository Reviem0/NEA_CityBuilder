// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"


#include "CB_LossScreen.generated.h"
UCLASS()
class NEA2_API UCB_LossScreen : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PointsLabel;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeLabel;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;
	
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void GetTime();
	void GetPoints();

	UFUNCTION()
	void OnMainMenuButtonClicked();
	
};
