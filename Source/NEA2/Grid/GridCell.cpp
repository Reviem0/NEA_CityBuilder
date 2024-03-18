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
void AGridCell::SetActive()
{
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);
	for (UStaticMeshComponent* MeshComponent : MeshComponents) {
		MeshComponent->SetMaterial(0, DefaultMAT);
	}
}
// Set Grid Cell as Occupied
void AGridCell::SetOccupied(EBuildingType NewBuildingType, AActor* NewActor)
{
	isOccupied = true;
	OccupyingType = NewBuildingType;
	OccupyingActor = NewActor;

	if (NewBuildingType == EBuildingType::Road) {
		bWalkable = true;
	}
	// DebugShowNeighbours();
}

void AGridCell::SetUnoccupied()
{
	isOccupied = false;
	OccupyingType = EBuildingType::None;
	OccupyingActor = nullptr;

	bWalkable = false;
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

// Set current Grid Cell MAT to debugMAT
void AGridCell::DebugSetMAT()
{
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);
	for (UStaticMeshComponent* MeshComponent : MeshComponents) {
		MeshComponent->SetMaterial(0, DebugMAT);
	}
}

void AGridCell::ResetMAT() {
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);
	for (UStaticMeshComponent* MeshComponent : MeshComponents) {
		MeshComponent->SetMaterial(0, DefaultMAT);
	}
}

void AGridCell::SetNeighbours()
{
	//Add neighbours to NEighbout array
	if (NNeighbour) {
		Neighbours.Add(*NNeighbour);
	}
	if (SNeighbour) {
		Neighbours.Add(*SNeighbour);
	}
	if (ENeighbour) {
		Neighbours.Add(*ENeighbour);
	}
	if (WNeighbour) {
		Neighbours.Add(*WNeighbour);
	}
}

int AGridCell::FCost() const { return GCost + HCost; }

void AGridCell::ResetPathfinding()
{
	GCost = 0;
	HCost = 0;
	Parent = nullptr;
}