// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_ClickComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UCB_ClickComponent::UCB_ClickComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UCB_ClickComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwner()->OnClicked.AddUniqueDynamic(this,&UCB_ClickComponent::OnMouseClick);
	if (GetOwner()->GetComponentByClass<UStaticMeshComponent>()){
		StaticMeshRef = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
		GetOwner()->OnBeginCursorOver.AddUniqueDynamic(this,&UCB_ClickComponent::OnCursorOver);
		GetOwner()->OnEndCursorOver.AddUniqueDynamic(this,&UCB_ClickComponent::OnEndCursorOver);
	}

	// ...
	
}


// Called every frame
void UCB_ClickComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCB_ClickComponent::OnMouseClick(AActor* Target, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked: %s"), *GetOwner()->GetName());
	isClicked = !isClicked;
	if (StaticMeshRef){
		if (isClicked){
		UE_LOG(LogTemp, Warning, TEXT("Selected: %s"), *GetOwner()->GetName());
		} 
		else {
			UE_LOG(LogTemp, Warning, TEXT("Unselected: %s"), *GetOwner()->GetName());
		}
	}
}

void UCB_ClickComponent::OnCursorOver(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("Hovering Over: %s"), *GetOwner()->GetName());
	StaticMeshRef->UPrimitiveComponent::SetRenderCustomDepth(true);
}

void UCB_ClickComponent::OnEndCursorOver(AActor *Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("End Hovering Over: %s"), *GetOwner()->GetName());
	StaticMeshRef->UPrimitiveComponent::SetRenderCustomDepth(false);
}

void UCB_ClickComponent::inPlacementMode() {
	DestroyComponent();
}