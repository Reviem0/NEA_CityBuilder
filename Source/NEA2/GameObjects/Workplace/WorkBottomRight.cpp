// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkBottomRight.h"

AWorkBottomRight::AWorkBottomRight()
{
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::Workplace;
}

void AWorkBottomRight::BeginPlay()
{
    Super::BeginPlay();
}

void AWorkBottomRight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
