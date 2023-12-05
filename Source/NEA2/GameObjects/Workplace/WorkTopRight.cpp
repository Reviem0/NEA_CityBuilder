// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkTopRight.h"

AWorkTopRight::AWorkTopRight()
{
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::Workplace;
}

void AWorkTopRight::BeginPlay()
{
    Super::BeginPlay();
}

void AWorkTopRight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
