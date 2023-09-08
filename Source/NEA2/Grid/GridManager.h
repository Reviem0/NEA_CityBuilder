// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"

#include "GridManager.generated.h"

UCLASS()
class NEA2_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GridSizeX = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GridSizeY = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WorldGridSize = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeightOffset = 0.5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGridCell> GridCell;


	TArray<AGridCell*> GridArray;

	// Populate Grid
	void PopulateGrid();

	// Populate Neighbours
	void PopulateNeighbours();

	FVector GetClosestGridPosition(FVector InPosition);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
