// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_BuildingAsset.h"
#include "Workplace/CB_WorkplaceAsset.h"
#include "../Enum/BuildingClassEnum.h"
#include "CB_OwnedRoadCell.h"
#include "Components/SplineComponent.h"

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
	void CarArrived(AActor* Car);
	
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int Points = 0;

	TArray<AActor*> HouseQueue;

	void OnWaitFinished();

	void CreatePath(AActor* House);
	
	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;

	void CreateSpline(TArray<AGridCell*> Path, AActor* House);

	void SendCar(AActor* House);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CarAI")
	TSubclassOf<AActor> CarToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
