// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_MainMenuGameMode.h"
#include "GameFramework/HUD.h"

void ACB_MainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();
    if (MainMenuWidgetClass)
    {
        UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();
        }
    }
    // enable cursor
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}
