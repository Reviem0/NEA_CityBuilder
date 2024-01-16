// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"

#include "../CB_Workplace.h"

#include "CB_CarAI.generated.h"

UCLASS()
class NEA2_API ACB_CarAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACB_CarAI();

	void FollowSpline(USplineComponent* Spline);

	UPROPERTY()
    FTimeline MovementTimeline;

	UPROPERTY()
	bool Returning = false;

    UFUNCTION()
    void TimelineFloatReturn(float value);

	UFUNCTION()
    void OnTimelineFinished();

    UPROPERTY()
    USplineComponent* SplineToFollow;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;


	UPROPERTY(VisibleAnywhere, Category = "Properties")
	AActor* OriginHouse;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	ACB_Workplace* DestinationWorkplace;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	EBuildingClass CarClass;
	
	UMaterialInterface* RedMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Red.HouseColour_Red'"));
	UMaterialInterface* BlueMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Blue.HouseColour_Blue'"));
	UMaterialInterface* GreenMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Green.HouseColour_Green'"));


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
