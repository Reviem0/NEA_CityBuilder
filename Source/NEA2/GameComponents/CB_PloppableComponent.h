// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Grid/GridManager.h"
#include "CB_PloppableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEA2_API UCB_PloppableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCB_PloppableComponent();

	bool IsPlacementValid;
	UMaterialInterface* ValidMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Game/GameObjects/Materials/M_Valid.M_Valid"));
	UMaterialInterface* InvalidMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Game/GameObjects/Materials/M_Invalid.M_Invalid"));

	UFUNCTION()
	void UpdateState();
	UFUNCTION()
	void RoadPlaceableCheck();
	
	AGridCell* LastGridRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
