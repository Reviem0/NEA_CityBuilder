// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CB_BuildingAsset.h"
#include "../CB_OwnedRoadCell.h"
#include "../../Enum/BuildingClassEnum.h"
#include "Components/SplineComponent.h"
#include "../CB_Workplace.h"
#include "../CarAI/CB_CarAI.h"

#include "CB_House.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_House : public ACB_BuildingAsset
{
	GENERATED_BODY()
public:
	ACB_House();

	UPROPERTY(EditAnywhere, Category = "Properties")
	EBuildingClass BuildingClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CarAI")
	TSubclassOf<ACB_CarAI> CarAI;

	UPROPERTY(EditAnywhere, Category = "Actors")
	TSubclassOf<ACB_OwnedRoadCell> RoadTileActor;

	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_OwnedRoadCell* RoadTileAsset;

	void CreatePath();

	void PathCheck();

	UPROPERTY(VisibleAnywhere, Category = "Workplaces")
	TArray<ACB_Workplace*> TargetWorkplaces;

	void CreateSpline(TArray<AGridCell*> Path, ACB_Workplace* TargetWorkplace);

	void SortWorkplaces();

	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;

	void CarArrived(ACB_CarAI* Car);
	
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int CarAvailability = 2;

	void AddTargetWorkplace(ACB_Workplace* Workplace);

	// Dictionary to store the distance between the house and the workplace
    TMap<ACB_Workplace*, TArray<AGridCell*>> WorkplaceDistances;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UMaterialInterface* RedMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Red.HouseColour_Red'"));
	UMaterialInterface* BlueMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Blue.HouseColour_Blue'"));
	UMaterialInterface* GreenMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Green.HouseColour_Green'"));
	UMaterialInterface* YellowMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Yellow.HouseColour_Yellow'"));


	
};
