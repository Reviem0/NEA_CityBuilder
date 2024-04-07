// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_LossScreen.h"
#include "Kismet/GameplayStatics.h"
#include "../NEA2GameModeBase.h"
#include "../GameManager.h"


void UCB_LossScreen::NativeConstruct()
{
    Super::NativeConstruct();
    MainMenuButton->OnClicked.AddUniqueDynamic(this, &UCB_LossScreen::OnMainMenuButtonClicked);
    GetTime();
    GetPoints();

}

void UCB_LossScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCB_LossScreen::GetTime()
{
    ANEA2GameModeBase* GameMode = Cast<ANEA2GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        int time = GameMode->time;
        // split time into hours and minutes
        int minutes = time / 60;
        int seconds = time % 60;
        FString TimeString = FString::Printf(TEXT("%02d:%02d"), minutes, seconds);
        TimeLabel->SetText(FText::FromString(TimeString));
    }
}

void UCB_LossScreen::GetPoints()
{
    AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
    if (GameManager) {
        int Score = GameManager->GetScore();
        PointsLabel->SetText(FText::AsNumber(Score));
    }
}

void UCB_LossScreen::OnMainMenuButtonClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}