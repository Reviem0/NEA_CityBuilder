// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Grid/GridManager.h"
#include "Components/ArrowComponent.h"

#include "CB_CameraControllerNew.generated.h"

UCLASS()
class NEA2_API ACameraControllerNew : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACameraControllerNew();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
