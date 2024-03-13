// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CB_BuildingAsset.h"
#include "../../Enum/BuildingClassEnum.h"

#include "CB_WorkplaceAsset.generated.h"


/**
 * 
 */
UCLASS()
class NEA2_API ACB_WorkplaceAsset : public ACB_BuildingAsset
{
	GENERATED_BODY()

public:
	ACB_WorkplaceAsset();
	UPROPERTY(VisibleAnywhere, Category = "References")
	ACB_BuildingAsset* WorkplaceRef;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	EBuildingClass BuildingClass;


	UMaterialInterface* RedMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Red.HouseColour_Red'"));
	UMaterialInterface* BlueMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Blue.HouseColour_Blue'"));
	UMaterialInterface* GreenMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Green.HouseColour_Green'"));
	UMaterialInterface* YellowMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Yellow.HouseColour_Yellow'"));



protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};
