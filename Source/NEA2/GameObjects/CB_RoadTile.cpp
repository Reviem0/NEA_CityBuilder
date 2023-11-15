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
<<<<<<< HEAD
    BuildingType = EBuildingType::Road;

	UpdateRoadMesh()
=======
    GridCellRef->SetOccupied(EBuildingType::Road, this)
>>>>>>> ccc3bf3eefee4708ab99455f164cf1cd1385d7c9
	
}

// Called every frame
void ACB_RoadTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACB_RoadTile::UpdateRoadMesh()
{
	// Get the static mesh component
	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (StaticMeshComponent)
	{
		// Check which neighbours are road tiles
		NorthR = Cast<ACB_RoadTile>(North);
		// If all neighbours are road tiles, use the cross mesh
		// If 3 neighbours are road tiles, use the T mesh
		// If 2 neighbours are road tiles, use the straight mesh
		// If 1 neighbour is a road tile, use the corner mesh
		// If 0 neighbours are road tiles, use the circle mesh


		
	}
}
