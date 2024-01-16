// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObjects/CB_Workplace.h"
#include "GameObjects/House/CB_House.h"

#include "GameManager.generated.h"

UCLASS()
class NEA2_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	TArray<AGridCell*> GridArray;
	int GridSizeX = 0;
	int GridSizeY = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACB_House> HouseClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACB_Workplace> WorkplaceClass;
	bool SpawnHouse(AGridCell* GridCell);
	void SpawnHouseAtRandomLocation();
	bool SpawnWorkplace(AGridCell* GridCell);
	void SpawnWorkplaceAtRandomLocation();

	int SpawnAttemptLimit = 10;

	void UpdatePath();

	TArray<ACB_House*> HouseArray;
	TArray<ACB_Workplace*> WorkplaceArray;

	void Init();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
