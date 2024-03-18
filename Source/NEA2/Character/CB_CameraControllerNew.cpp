// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_CameraControllerNew.h"
#include "../Grid/GridManager.h"
#include "../Grid/GridCell.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ACameraControllerNew::ACameraControllerNew()
{

}

// Called when the game starts or when spawned
void ACameraControllerNew::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACameraControllerNew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACameraControllerNew::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

