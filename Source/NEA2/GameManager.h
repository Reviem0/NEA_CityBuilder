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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HouseClasses")
	TSubclassOf<ACB_House> HouseRedClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HouseClasses")
	TSubclassOf<ACB_House> HouseBlueClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HouseClasses")
	TSubclassOf<ACB_House> HouseGreenClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HouseClasses")
	TSubclassOf<ACB_House> HouseYellowClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WorkplaceClasses")
	TSubclassOf<ACB_Workplace> WorkplaceRedClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WorkplaceClasses")
	TSubclassOf<ACB_Workplace> WorkplaceBlueClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WorkplaceClasses")
	TSubclassOf<ACB_Workplace> WorkplaceGreenClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WorkplaceClasses")
	TSubclassOf<ACB_Workplace> WorkplaceYellowClass;

	TArray<EBuildingClass> AvailableColours;
	TArray<EBuildingClass> RemainingColours;

	bool SpawnHouse(AGridCell* GridCell, EBuildingClass BuildingClass);
	bool SpawnHouseAtRandomLocation(EBuildingClass BuildingClass = EBuildingClass::None);
	bool SpawnWorkplace(AGridCell* GridCell, EBuildingClass BuildingClass);
	bool SpawnWorkplaceAtRandomLocation(EBuildingClass BuildingClass = EBuildingClass::None);

	void SpawnColourSet(EBuildingClass BuildingClass = EBuildingClass::None);

	int SpawnAttemptLimit = 10;

	void UpdatePath();

	TArray<ACB_House*> HouseArray;
	TArray<ACB_Workplace*> WorkplaceArray;

	void Init();

	UFUNCTION(BlueprintCallable)
	void AddScore(int Score);

	void ScoreFunction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Score")
	int TotalScore;

};
