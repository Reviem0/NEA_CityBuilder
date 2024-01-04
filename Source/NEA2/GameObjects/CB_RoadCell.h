// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_RoadTile.h"
#include "CB_RoadCell.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_RoadCell : public ACB_RoadTile
{
	GENERATED_BODY()

public:
	ACB_RoadCell();

	UPROPERTY(VisibleAnywhere, Category = "Reference")
	ACB_RoadTile* RoadRef;

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

	UPROPERTY(EditDefaultsOnly, Category = "RoadMesh")
	TSubclassOf<ACB_RoadTile> RoadCross;

	UPROPERTY(EditDefaultsOnly, Category = "RoadMesh")
	TSubclassOf<ACB_RoadTile> RoadT;

	UPROPERTY(EditDefaultsOnly, Category = "RoadMesh")
	TSubclassOf<ACB_RoadTile> RoadStraight;

	UPROPERTY(EditDefaultsOnly, Category = "RoadMesh")
	TSubclassOf<ACB_RoadTile> RoadCorner;

	UPROPERTY(EditDefaultsOnly, Category = "RoadMesh")
	TSubclassOf<ACB_RoadTile> RoadEnd;

	UPROPERTY(EditDefaultsOnly, Category = "RoadMesh")
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

	
};
