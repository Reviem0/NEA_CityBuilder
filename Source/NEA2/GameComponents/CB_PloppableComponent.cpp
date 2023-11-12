// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_PloppableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Grid/GridManager.h"


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
	
	GetOwner()->OnActorBeginOverlap.AddDynamic(this, &UCB_PloppableComponent::OnOverlap);
	GetOwner()->OnActorEndOverlap.AddDynamic(this, &UCB_PloppableComponent::OnOverlap);
}


// Called every frame
void UCB_PloppableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateState();
	// ...
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

void UCB_PloppableComponent::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Display, TEXT("OVERLAP"));
	UpdateState();
}