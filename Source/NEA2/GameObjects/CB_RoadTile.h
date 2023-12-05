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

	UPROPERTY(VisibleAnywhere, Category = "Road")
	bool OldNorthR;
	UPROPERTY(VisibleAnywhere, Category = "Road")
	bool OldSouthR;
	UPROPERTY(VisibleAnywhere, Category = "Road")
	bool OldEastR;
	UPROPERTY(VisibleAnywhere, Category = "Road")
	bool OldWestR;

	UPROPERTY(VisibleAnywhere)
	bool IsUpdatingMesh = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACB_RoadTile> RoadCross;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACB_RoadTile> RoadT;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACB_RoadTile> RoadStraight;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACB_RoadTile> RoadCorner;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACB_RoadTile> RoadEnd;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACB_RoadTile> RoadCircle;


	UFUNCTION(BlueprintCallable)
	void UpdateRoadMesh();

	void UpdateNeighbours();

	AGridCell* LastGridRef;

	void DestroyRoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void SpawnNewActor(UClass *ActorClass, FRotator Rotation);

    void NewSpawn(ACB_RoadTile* NewRoadTile);

};
