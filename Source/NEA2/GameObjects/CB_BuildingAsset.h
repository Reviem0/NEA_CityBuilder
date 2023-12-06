// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enum/BuildingTypeEnum.h"
#include "../Enum/OwnershipEnum.h"
#include "../Grid/GridCell.h"
#include "CB_BuildingAsset.generated.h"

UCLASS()
class NEA2_API ACB_BuildingAsset : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACB_BuildingAsset();

	UPROPERTY(EditAnywhere)
	bool isPlop = true;

	UPROPERTY()
	EBuildingType BuildingType {EBuildingType::Placed};

	UPROPERTY(VisibleAnywhere)
	EOwnership Ownership {EOwnership::None};

	UPROPERTY(EditAnywhere)
	AGridCell* GridCellRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
