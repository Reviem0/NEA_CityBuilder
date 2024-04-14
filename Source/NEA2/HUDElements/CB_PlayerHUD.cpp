// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_PlayerHUD.h"
#include "../Character/CB_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManager.h"


void UCB_PlayerHUD::NativeConstruct()
{
    Super::NativeConstruct();
    FastForwardButton->OnClicked.AddUniqueDynamic(this, &UCB_PlayerHUD::OnFastForwardButtonClicked);
    NormalButton->OnClicked.AddUniqueDynamic(this, &UCB_PlayerHUD::OnNormalButtonClicked);

}

void UCB_PlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    UpdateRoadInventory();
    UpdateScore();

}
void UCB_PlayerHUD::UpdateTime(int time)
{
    // split time into hours and minutes
    int minutes = time / 60;
    int seconds = time % 60;
    FString TimeString = FString::Printf(TEXT("%02d:%02d"), minutes, seconds);
    TimeLabel->SetText(FText::FromString(TimeString));
}

void UCB_PlayerHUD::UpdateRoadInventory()
{
    // Get the player controller
    ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    // Get the road inventory
    int Inventory = 0;
    if (PlayerController) {
        Inventory = PlayerController->RoadInventory;
    }

    // Update the road inventory label
    RoadInventoryLabel->SetText(FText::AsNumber(Inventory));
}

void UCB_PlayerHUD::UpdateScore()
{
    // Get game manager
    int Score = 0;
    AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
    if (GameManager) {
        Score = GameManager->GetScore();
    }
    PlayerScoreLabel->SetText(FText::AsNumber(Score));
}

void UCB_PlayerHUD::OnFastForwardButtonClicked()
{
    FastForward();

}

void UCB_PlayerHUD::OnNormalButtonClicked()
{
    NormalSpeed();
}
void UCB_PlayerHUD::FastForward()
{
    UE_LOG(LogTemp, Warning, TEXT("Fast Forward"));
    // Get the player controller
    ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PlayerController) {
        PlayerController->IncreaseSpeed();
    }
    
}

void UCB_PlayerHUD::NormalSpeed()
{
    UE_LOG(LogTemp, Warning, TEXT("Normal Speed"));
    // Get the player controller
    ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PlayerController) {
        PlayerController->ResetSpeed();
    }
}
