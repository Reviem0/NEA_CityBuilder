// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enum/BuildingTypeEnum.h"

#include "GridCell.generated.h"

UCLASS()
class NEA2_API AGridCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridCell();

	AGridCell** NNeighbour;
	AGridCell** SNeighbour;
	AGridCell** ENeighbour;
	AGridCell** WNeighbour;
	bool isOccupied = false;

	UMaterialInterface* DebugMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/Grid/TextureAssets/DebugTile_Mat.DebugTile_Mat'"));
	
	UPROPERTY(EditAnywhere)
	EBuildingType OccupyingType {EBuildingType::None};
	UPROPERTY(EditAnywhere)
	AActor* OccupyingActor;

	void SetOccupied(EBuildingType NewBuildingType, AActor* NewActor);
    void SetUnoccupied();

    void DebugShowNeighbours();
	void DebugSetMAT();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
