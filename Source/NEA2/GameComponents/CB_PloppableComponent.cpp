// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_PloppableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../GameObjects/CB_BuildingAsset.h"
#include "../Enum/BuildingTypeEnum.h"
#include "../Character/CB_PlayerController.h"


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
	UpdateState();
}

// Called every frame
void UCB_PloppableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!LastGridRef){
		LastGridRef = (Cast<ACB_BuildingAsset>(GetOwner()))->GridCellRef;
	}
	// If GridRef Changes, UpdateState
	if (LastGridRef != (Cast<ACB_BuildingAsset>(GetOwner()))->GridCellRef){
		UpdateState();
		LastGridRef = (Cast<ACB_BuildingAsset>(GetOwner()))->GridCellRef;
	}

}

void UCB_PloppableComponent::UpdateState()
{
	ACB_BuildingAsset* Owner = Cast<ACB_BuildingAsset>(GetOwner());
	AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
	ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (GridManager->GridArray.Num() != 0) {
		if (GridManager->PlayGridArray.Contains(Owner->GridCellRef)){ // If the GridCellRef is in the PlayGridArray
			if (!(Owner->GridCellRef->isOccupied) && PlayerController->RoadInventory > 0) { // If the GridCellRef is not occupied and the player has road inventory
				IsPlacementValid = true; // Placement is valid
			} else {
				IsPlacementValid = false; // Placement is invalid
		}
		} else {
			IsPlacementValid = false; // Placement is invalid
		}
	}

	// Change Material based on Placement Validity
	TArray<UStaticMeshComponent*> MeshComponents; 
	GetOwner()->GetComponents<UStaticMeshComponent>(MeshComponents);
	for (UStaticMeshComponent* MeshComponent : MeshComponents) {
		if (IsPlacementValid) {
			MeshComponent->SetMaterial(0, ValidMAT); // Set Material to ValidMAT
		} else {
			MeshComponent->SetMaterial(0, InvalidMAT); // Set Material to InvalidMAT
		}
	}
}

void UCB_PloppableComponent::RoadPlaceableCheck()
{
	ACB_BuildingAsset* Owner = Cast<ACB_BuildingAsset>(GetOwner());

	// Validation Checks

	// If Owner doesn't exist, return
	if (!Owner){
		return;
	}

	// If object is not a road, return
	if (Owner->BuildingType != EBuildingType::Road){
		return;
	}
	// If Owner doesnt have a GridRef, return
	if (!(Owner->GridCellRef)) {
		return;
	}

	//North
	if (Owner->GridCellRef->NNeighbour)
	{
		// North
		AGridCell* NorthCell = (*(Owner->GridCellRef->NNeighbour));
		bool North = NorthCell->OccupyingType == EBuildingType::Road;
		// North-West
		if (Owner->GridCellRef->WNeighbour){
			AGridCell* WestCell = (*(Owner->GridCellRef->WNeighbour));
			bool West = WestCell->OccupyingType == EBuildingType::Road;
	  		// If North is road and West is road:
			if (North && West) {
				if (!(NorthCell->WNeighbour)) {
					return;
				}
				// Then if NorthWest is road, placement is invalid
				if ((*(NorthCell->WNeighbour))->OccupyingType == EBuildingType::Road) {
					IsPlacementValid = false;
					return;
				}
			}
		}

		//North-East
		if (Owner->GridCellRef->ENeighbour){
			AGridCell* EastCell = (*(Owner->GridCellRef->ENeighbour));
			bool East = EastCell->OccupyingType == EBuildingType::Road;
			// If North is road and East is road:
			if (North && East) {
				if (!(NorthCell->ENeighbour)) {
					return;
				}
				// Then if NorthEast is road, placement is invalid
				if ((*(NorthCell->ENeighbour))->OccupyingType == EBuildingType::Road) {
					IsPlacementValid = false;
					return;
				}
			}
		}
	}

 	//South
	if (Owner->GridCellRef->SNeighbour) 
	{
		AGridCell* SouthCell = (*(Owner->GridCellRef->SNeighbour));
		bool South = SouthCell ->OccupyingType == EBuildingType::Road;
		//South-West
		if (Owner->GridCellRef->WNeighbour){
			AGridCell* WestCell = (*(Owner->GridCellRef->WNeighbour));
			bool West = WestCell->OccupyingType == EBuildingType::Road;
			// If South is road and West is road:
			if (South && West) {
				if (!(SouthCell->WNeighbour)) {
					return;
				}
				// Then if SouthWest is road, placement is invalid
				if ((*(SouthCell->WNeighbour))->OccupyingType == EBuildingType::Road) {
					IsPlacementValid = false;
					return;
				}
			}
		}

		//South-East
		if (Owner->GridCellRef->ENeighbour){
			AGridCell* EastCell = (*(Owner->GridCellRef->ENeighbour));
			bool East = EastCell->OccupyingType == EBuildingType::Road;
			// If South is road and East is road:
			if (South && East) {
				if (!(SouthCell->ENeighbour)) {
					return;
				}
				// Then if SouthEast is road, placement is invalid
				if ((*(SouthCell->ENeighbour))->OccupyingType == EBuildingType::Road) {
					IsPlacementValid = false;
					return;
				}
			}
		}
	}
	return;
}
