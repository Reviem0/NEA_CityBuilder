// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_BuildingAsset.h"
#include "Workplace/CB_WorkplaceAsset.h"
#include "../Enum/BuildingClassEnum.h"
#include "CB_OwnedRoadCell.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"

#include "CB_Workplace.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_Workplace : public ACB_BuildingAsset
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACB_Workplace();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Properties")
	EBuildingClass BuildingClass;

	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_WorkplaceAsset> TopRightActor;
	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_WorkplaceAsset> TopLeftActor;
	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_WorkplaceAsset> BottomRightActor;
	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_WorkplaceAsset> BottomLeftActor;
	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_OwnedRoadCell> RoadTileActor;

	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_WorkplaceAsset* TopRightAsset;
	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_WorkplaceAsset* TopLeftAsset;
	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_WorkplaceAsset* BottomRightAsset;
	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_WorkplaceAsset* BottomLeftAsset;
	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_OwnedRoadCell* RoadTileAsset;

	UFUNCTION(BlueprintCallable)
	void CarArrived(class ACB_CarAI* CarAI);

	TArray<class ACB_House*> HouseQueue;
	void EnqueueCar(class ACB_House* House);
	void DequeueCar();
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	int HoldingCapacity = 5;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int HoldingCurrent = 0;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float cooldown = 15.0f;


	void AddScore();

	
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int Points = 0;
	

	bool CreatePath(class ACB_House* House);
	void CreateSpline(TArray<AGridCell*> Path, class ACB_House* House);
	
	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;

	int Goal;
	int CurrentScore = 0;
	void IncreaseGoal();

	
	UPROPERTY(EditAnywhere, Category = "Stats")
	bool IsFull = false;

	

	void SendCar(class ACB_House* House);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CarAI")
	TSubclassOf<class ACB_CarAI> CarToSpawn;

	void DestroyWorkplace();


	UPROPERTY(EditAnywhere, Category = "HUDElements")
	bool isCritical = false;

	FTimerHandle GoalTimerHandle;
	FTimerHandle CriticalTimerHandle;

	UPROPERTY(EditAnywhere, Category = "LossConditions")
	float CriticalTime = 60;
	UPROPERTY(EditAnywhere, Category = "LossConditions")
	float GoalTimer = 60*5;

	void LossCondition();
	void GoalNotMet();
	
	bool isLost = false;

	UPROPERTY(VisibleAnywhere, Category = "HUDElements")
	UWidgetComponent* CriticalBarComp;
	
	float GetCriticalValue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
