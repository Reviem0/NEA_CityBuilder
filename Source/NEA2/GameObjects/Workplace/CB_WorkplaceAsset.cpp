// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_WorkplaceAsset.h"

// Sets default values
ACB_WorkplaceAsset::ACB_WorkplaceAsset()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::Workplace;
    
}

// Called when the game starts or when spawned
void ACB_WorkplaceAsset::BeginPlay()
{
    Super::BeginPlay();
    

}

// Called every frame
void ACB_WorkplaceAsset::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

