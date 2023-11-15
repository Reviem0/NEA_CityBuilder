// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GameObjects/Roads/RoadCross.h"
#include "../GameObjects/Roads/RoadT.h"
#include "../GameObjects/Roads/RoadStraight.h"
#include "../GameObjects/Roads/RoadCorner.h"
#include "../GameObjects/Roads/RoadEnd.h"
#include "../GameObjects/Roads/RoadCircle.h"

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

	bool isOcc = false;

	bool IsUpdatingMesh = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARoadCross> RoadCross;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARoadT> RoadT;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARoadStraight> RoadStraight;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARoadCorner> RoadCorner;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARoadEnd> RoadEnd;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARoadCircle> RoadCircle;


	UFUNCTION(BlueprintCallable)
	void UpdateRoadMesh(bool neighbourUpdate);

	void UpdateNeighbours();

	AGridCell* LastGridRef;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
