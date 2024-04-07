// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_MainMenuGameMode.h"
#include "GameFramework/HUD.h"

void ACB_MainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();
    // enable cursor
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
    // check if logged in
    if (UFirebaseAuthenticationSubsystem::GetFirebaseUser() != nullptr)
    {
        // if logged in, go to main menu
        LoadMainMenu();
    }
    else
    {
        
    }
}

void ACB_MainMenuGameMode::LoadMainMenu()
{
    if (MainMenuWidgetClass)
    {
        UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();
        }
    }
}
