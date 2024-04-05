// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "CB_PlayerHUD.generated.h"
UCLASS()
class NEA2_API UCB_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateRoadInventory();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RoadInventoryLabel;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* FastForwardButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* NormalButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeLabel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerScoreLabel;

	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateTime(int time);

	void FastForward();
	void NormalSpeed();

	void UpdateScore();

	UFUNCTION()
	void OnFastForwardButtonClicked();
	UFUNCTION()
	void OnNormalButtonClicked();

	
};
