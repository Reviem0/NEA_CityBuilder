// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

#include "CB_LoginPage.generated.h"
UCLASS()
class NEA2_API UCB_LoginPage : public UUserWidget
{
	GENERATED_BODY()
public:

	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* EmailTextBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* PasswordTextBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* LoginButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* RegisterButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ResetPasswordButton;
	
	UFUNCTION()
	void OnLoginButtonClicked();
	UFUNCTION()
	void OnRegisterButtonClicked();
	UFUNCTION()
	void OnResetPasswordButtonClicked();


};
