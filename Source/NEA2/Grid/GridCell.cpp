// Fill out your copyright notice in the Description page of Project Settings.


#include "GridCell.h"

// Sets default values
AGridCell::AGridCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridCell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Set Grid Cell as Occupied
void AGridCell::SetOccupied(EBuildingType NewBuildingType, AActor* NewActor)
{
	isOccupied = true;
	BuildingType = NewBuildingType;
	OccupyingActor = NewActor;
	DebugShowNeighbours();
}

// Show Neighbours for debugging
void AGridCell::DebugShowNeighbours()
{
	if (NNeighbour){
		(*NNeighbour)->DebugSetMAT();
	}
	if (SNeighbour){
		(*SNeighbour)->DebugSetMAT();
	}
	if (ENeighbour){
		(*ENeighbour)->DebugSetMAT();
	}
	if (WNeighbour){
		(*WNeighbour)->DebugSetMAT();
	}
}

// Set current Grid Cell to debugMAT
void AGridCell::DebugSetMAT()
{
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);
	for (UStaticMeshComponent* MeshComponent : MeshComponents) {
		MeshComponent->SetMaterial(0, DebugMAT);
	}
}
