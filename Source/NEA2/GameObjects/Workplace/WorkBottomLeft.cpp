// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkBottomLeft.h"

AWorkBottomLeft::AWorkBottomLeft()
{
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::Workplace;
}

void AWorkBottomLeft::BeginPlay()
{
    Super::BeginPlay();
}

void AWorkBottomLeft::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


