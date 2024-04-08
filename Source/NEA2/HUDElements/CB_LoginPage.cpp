// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_LoginPage.h"
#include "../Character/CB_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManager.h"
#include "Kismet/KismetSystemLibrary.h"


void UCB_LoginPage::NativeConstruct()
{
    Super::NativeConstruct();
    LoginButton->OnClicked.AddDynamic(this, &UCB_LoginPage::OnLoginButtonClicked);
    RegisterButton->OnClicked.AddDynamic(this, &UCB_LoginPage::OnRegisterButtonClicked);
    ResetPasswordButton->OnClicked.AddDynamic(this, &UCB_LoginPage::OnResetPasswordButtonClicked);


}

void UCB_LoginPage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCB_LoginPage::OnLoginButtonClicked()
{
}

void UCB_LoginPage::OnRegisterButtonClicked()
{
}

void UCB_LoginPage::OnResetPasswordButtonClicked()
{
}
