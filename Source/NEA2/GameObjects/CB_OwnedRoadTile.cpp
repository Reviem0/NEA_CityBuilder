// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_OwnedRoadTile.h"

ACB_OwnedRoadTile::ACB_OwnedRoadTile()
{
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::Road;
    Ownership = EOwnership::Workplace;
    
}

// Sets default values
void ACB_OwnedRoadTile::BeginPlay(){
    Super::BeginPlay();

}

// Called every frame
void ACB_OwnedRoadTile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}