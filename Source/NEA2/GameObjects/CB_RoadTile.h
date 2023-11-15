// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_BuildingAsset.h"
#include "CB_RoadTile.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_RoadTile : public ACB_BuildingAsset
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACB_RoadTile();

	UPROPERTY(EditAnywhere)
	UStaticMesh* RoadCircle;

	UPROPERTY(EditAnywhere)
	UStaticMesh* RoadVert;

	UPROPERTY(EditAnywhere)
	UStaticMesh* RoadHoriz;

	UPROPERTY(EditAnywhere)
	UStaticMesh* RoadCorner1;

	UPROPERTY(EditAnywhere)
	UStaticMesh* RoadCorner2;

	UPROPERTY(EditAnywhere)
	UStaticMesh* RoadCorner3;

	UPROPERTY(EditAnywhere)
	UStaticMesh* RoadCorner4;

	UPROPERTY(EditAnywhere)
	UStaticMesh* RoadT;

	UPROPERTY(EditAnywhere)
	UStaticMesh* RoadCross;

	UFUNCTION(BlueprintCallable)
	void UpdateRoadMesh();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
