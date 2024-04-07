// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_MainMenu.h"
#include "../Character/CB_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManager.h"
#include "Kismet/KismetSystemLibrary.h"


void UCB_MainMenu::NativeConstruct()
{
    Super::NativeConstruct();
    StartGameButton->OnClicked.AddUniqueDynamic(this, &UCB_MainMenu::OnStartGameButtonClicked);
    QuitButton->OnClicked.AddUniqueDynamic(this, &UCB_MainMenu::OnQuitButtonClicked);

}

void UCB_MainMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCB_MainMenu::OnStartGameButtonClicked()
{
    // Start Level
    UGameplayStatics::OpenLevel(GetWorld(), "MAIN");
}

void UCB_MainMenu::OnQuitButtonClicked()
{
    // Quit Game
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}