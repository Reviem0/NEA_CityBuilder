// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/EditableTextBox.h"
#include <Http.h>

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

	int FinalScore;
	int FinalTime;

	void GetTime();
	void GetPoints();

	UFUNCTION()
	void OnMainMenuButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player2;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player3;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player4;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player5;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player6;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player7;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player8;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player9;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player10;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score_1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score_2;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score_3;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score_4;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score_5;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score_6;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score_7;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score_8;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score_9;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Score_10;

	TMap<UTextBlock*, UTextBlock*> PlayerScoreMap;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UButton* SubmitScoreButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ErrorLabel;

	UFUNCTION()
	void OnSubmitScoreButtonClicked();

	void GetLeaderboard();

	void UpdateLeaderboardEntry(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);





	
};
