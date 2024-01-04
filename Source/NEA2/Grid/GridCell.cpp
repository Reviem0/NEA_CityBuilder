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
	OccupyingType = NewBuildingType;
	OccupyingActor = NewActor;
	DebugShowNeighbours();
}

void AGridCell::SetUnoccupied()
{
	isOccupied = false;
	OccupyingType = EBuildingType::None;
	OccupyingActor = nullptr;
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

void AGridCell::SetNeighbours()
{
	//Add neighbours to NEighbout array
	Neighbours.Add(*NNeighbour);
	Neighbours.Add(*SNeighbour);
	Neighbours.Add(*ENeighbour);
	Neighbours.Add(*WNeighbour);
}

int AGridCell::FCost() const
{
    return GCost + HCost;
}

void AGridCell::ResetPathfinding()
{
	GCost = 0;
	HCost = 0;
	Parent = nullptr;
}