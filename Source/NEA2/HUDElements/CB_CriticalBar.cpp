// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_CriticalBar.h"
#include "../Character/CB_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "../GameObjects/CB_Workplace.h"


void UCB_CriticalBar::NativeConstruct()
{
    Super::NativeConstruct();

}

void UCB_CriticalBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (OwnerWorkplace.IsValid())
    {
        if (OwnerWorkplace->isCritical){
            EnableCriticalBar(true);
            SetCriticalValue(OwnerWorkplace->GetCriticalValue());
        }
        else
        {
            EnableCriticalBar(false);
        }
        CurrentPointsLabel->SetText(FText::FromString(FString::FromInt(OwnerWorkplace->CurrentScore)));
        GoalPointsLabel->SetText(FText::FromString(FString::FromInt(OwnerWorkplace->Goal)));
    } else {
        EnableCriticalBar(false);
    }

}

void UCB_CriticalBar::SetCriticalValue(float Value)
{
    CriticalBar->SetPercent(Value);
}

void UCB_CriticalBar::EnableCriticalBar(bool bEnable)
{
    CriticalBar->SetVisibility(bEnable ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
