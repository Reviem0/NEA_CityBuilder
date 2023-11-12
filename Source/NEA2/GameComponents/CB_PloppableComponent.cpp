// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_PloppableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../GameObjects/CB_BuildingAsset.h"
#include "../Enum/BuildingTypeEnum.h"


// Sets default values for this component's properties
UCB_PloppableComponent::UCB_PloppableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

	// ...
}

// Called when the game starts
void UCB_PloppableComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCB_PloppableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!LastGridRef){
		LastGridRef = (Cast<ACB_BuildingAsset>(GetOwner()))->GridCellRef;
	}
	if (LastGridRef != (Cast<ACB_BuildingAsset>(GetOwner()))->GridCellRef){
		UpdateState();
	}

}

void UCB_PloppableComponent::UpdateState()
{
	AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
	if (GridManager->GridArray.Num() != 0) {
		if (!(GridManager->GetClosestGridCell(GetOwner()->GetActorLocation())->isOccupied)) {
			IsPlacementValid = true;
		} else {
			IsPlacementValid = false;
		}
		RoadPlaceableCheck();
	}

/* 	TArray<AActor*> OverlappingActors;
	GetOwner()->GetOverlappingActors(OverlappingActors, AActor::StaticClass());
	IsPlacementValid = OverlappingActors.Num() == 0; */

	TArray<UStaticMeshComponent*> MeshComponents; 
	GetOwner()->GetComponents<UStaticMeshComponent>(MeshComponents);
	for (UStaticMeshComponent* MeshComponent : MeshComponents) {
		if (IsPlacementValid) {
			MeshComponent->SetMaterial(0, ValidMAT);
		} else {
			MeshComponent->SetMaterial(0, InvalidMAT);
		}
	}
}

void UCB_PloppableComponent::RoadPlaceableCheck()
{
	ACB_BuildingAsset* Owner = Cast<ACB_BuildingAsset>(GetOwner());
	if (Owner == nullptr){
		return;
	}

	if (!(Owner->BuildingType == EBuildingType::Road)){
		return;
	}
	if (!(Owner->GridCellRef)) {
		return;
	}

	/* if (!(Owner->GridCellRef->NNeighbour) || !(Owner->GridCellRef->SNeighbour) || !(Owner->GridCellRef->ENeighbour) || !(Owner->GridCellRef->WNeighbour)) {
		UE_LOG(LogTemp, Display, TEXT("Your message"));
		return;
	} */

	//N
	if (Owner->GridCellRef->NNeighbour)
	{
		AGridCell* NorthCell = (*(Owner->GridCellRef->NNeighbour));
		bool North = NorthCell->OccupyingType == EBuildingType::Road;
		//NW
		if (Owner->GridCellRef->WNeighbour){
			AGridCell* WestCell = (*(Owner->GridCellRef->WNeighbour));
			bool West = WestCell->OccupyingType == EBuildingType::Road;
			if (North && West) {
				if (!(NorthCell->WNeighbour)) {
					return;
				}
				if ((*(NorthCell->WNeighbour))->OccupyingType == EBuildingType::Road) {
					IsPlacementValid = false;
					return;
				}
			}
			
		}

		 //NE
		if (Owner->GridCellRef->ENeighbour){
			AGridCell* EastCell = (*(Owner->GridCellRef->ENeighbour));
			bool East = EastCell->OccupyingType == EBuildingType::Road;
			if (North && East) {
				if (!(NorthCell->ENeighbour)) {
					return;
				}
				if ((*(NorthCell->ENeighbour))->OccupyingType == EBuildingType::Road) {
					IsPlacementValid = false;
					return;
				}
			}
		}
	}

 	//S
	if (Owner->GridCellRef->SNeighbour) 
	{
		AGridCell* SouthCell = (*(Owner->GridCellRef->SNeighbour));
		bool South = SouthCell ->OccupyingType == EBuildingType::Road;
		//SW
		if (Owner->GridCellRef->WNeighbour){
			AGridCell* WestCell = (*(Owner->GridCellRef->WNeighbour));
			bool West = WestCell->OccupyingType == EBuildingType::Road;
			if (South && West) {
				if (!(SouthCell->WNeighbour)) {
					return;
				}
				if ((*(SouthCell->WNeighbour))->OccupyingType == EBuildingType::Road) {
					IsPlacementValid = false;
					return;
				}
			}
		}

		//SE
		if (Owner->GridCellRef->ENeighbour){
			AGridCell* EastCell = (*(Owner->GridCellRef->ENeighbour));
			bool East = EastCell->OccupyingType == EBuildingType::Road;
			if (South && East) {
				if (!(SouthCell->ENeighbour)) {
					return;
				}
				if ((*(SouthCell->ENeighbour))->OccupyingType == EBuildingType::Road) {
					IsPlacementValid = false;
					return;
				}
			}
		}
	}
	return;
}
