// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"

#include "CB_CriticalBar.generated.h"
UCLASS()
class NEA2_API UCB_CriticalBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CriticalBar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentPointsLabel;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoalPointsLabel;
	
	void NativeConstruct() override;

	void SetCriticalValue(float Value);

	void EnableCriticalBar(bool bEnable);

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TWeakObjectPtr<class ACB_Workplace> OwnerWorkplace;
	void SetOwnerWorkplace(class ACB_Workplace* Workplace) { OwnerWorkplace = Workplace;}
	
};
