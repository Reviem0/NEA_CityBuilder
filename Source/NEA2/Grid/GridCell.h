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

	UPROPERTY(VisibleAnywhere)
	AActor* Manager = nullptr;
	
	
	AGridCell** NNeighbour;
	AGridCell** SNeighbour;
	AGridCell** ENeighbour;
	AGridCell** WNeighbour;
	bool isOccupied = false;

	UMaterialInterface* DebugMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/Grid/TextureAssets/DebugTile_Mat.DebugTile_Mat'"));
	UMaterialInterface* DefaultMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/Grid/TextureAssets/Tile_Mat.Tile_Mat'"));
	
	UPROPERTY(EditAnywhere)
	EBuildingType OccupyingType {EBuildingType::None};
	UPROPERTY(EditAnywhere)
	AActor* OccupyingActor;

	void SetOccupied(EBuildingType NewBuildingType, AActor* NewActor);
    void SetUnoccupied();

    void DebugShowNeighbours();
	void DebugSetMAT();

	void ResetMAT();

	//Neighbours
	TArray<AGridCell*> Neighbours;
	void SetNeighbours();

	//A* Pathfinding
    int FCost() const;
	UPROPERTY(EditAnywhere, Category = "Pathfinding")
	int GCost = 0;
	UPROPERTY(EditAnywhere, Category = "Pathfinding")
	int HCost = 0;
	UPROPERTY(EditAnywhere, Category = "Pathfinding")
	bool bWalkable = false;
	UPROPERTY(EditAnywhere, Category = "Pathfinding")
	AGridCell* Parent = nullptr;
	void ResetPathfinding();
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
