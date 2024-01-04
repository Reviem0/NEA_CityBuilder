// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"
#include "../GameManager.h"

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
	int WorldGridSize = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeightOffset = 0.5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGridCell> GridCell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGameManager> GameManagerClass;



	TArray<AGridCell*> GridArray;

	// Populate Grid
	void PopulateGrid();

	// Populate Neighbours
	void PopulateGridNeighbours();
	void SetNeighbourArray();

	//A* Pathfinding
	TArray<AGridCell*> FindPath(AGridCell* Start, AGridCell* End);
	int GetDistance(AGridCell* cellA, AGridCell* cellB);
	TArray<AGridCell*> RetracePath(AGridCell *startCell, AGridCell *endCell);
	void PathTest(int Start, int End);
	UPROPERTY(EditAnywhere, Category = "Pathfinding")
	int PathStart = 0;
	UPROPERTY(EditAnywhere, Category = "Pathfinding")
	int PathEnd = 0;


	// Init Game Manager
	void InitGameManager();

	FVector GetClosestGridPosition(FVector InPosition);
	AGridCell* GetClosestGridCell(FVector InPosition);

	FVector GetGridScale();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


