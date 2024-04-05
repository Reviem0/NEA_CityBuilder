// Copyright Epic Games, Inc. All Rights Reserved.


#include "NEA2GameModeBase.h"
#include "GameFramework/HUD.h"

ANEA2GameModeBase::ANEA2GameModeBase()
{
}

void ANEA2GameModeBase::BeginPlay()
{
    PlayerHUD = CreateWidget<UCB_PlayerHUD>(GetWorld(), PlayerHUDClass);
    if (PlayerHUD) {
        PlayerHUD->AddToViewport();
    }

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANEA2GameModeBase::UpdateTime, 1.0f, true);
}

void ANEA2GameModeBase::UpdateTime()
{
    time += 1;
    if (PlayerHUD) {
        PlayerHUD->UpdateTime(time);
    }
}