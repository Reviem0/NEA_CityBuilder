// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_RoadTile.h"

ACB_RoadTile::ACB_RoadTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACB_RoadTile::BeginPlay()
{
	Super::BeginPlay();
    GridCellRef->SetOccupied(EBuildingType::Road, this)
	
}

// Called every frame
void ACB_RoadTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

