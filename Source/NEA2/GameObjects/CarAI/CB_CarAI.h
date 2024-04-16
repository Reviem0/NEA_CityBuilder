// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"

#include "../CB_Workplace.h"

#include "CB_CarAI.generated.h"

UCLASS()
class NEA2_API ACB_CarAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACB_CarAI();

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CarMesh;
	// Create triggerbox for car to stop at
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;

	ACB_CarAI* CollidingCar;

	bool Overlapping = false;

	void FollowSpline(USplineComponent* Spline);

	UPROPERTY()
    FTimeline MovementTimeline;


	UPROPERTY(VisibleAnywhere, Category = "Properties")
	bool Returning = false;

    UFUNCTION()
    void TimelineFloatReturn(float value);

	UFUNCTION()
    void OnTimelineFinished();

	void PlayCar();

    UPROPERTY()
    USplineComponent* SplineToFollow;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	float speed = 200.f;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	class ACB_House* OriginHouse;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	ACB_Workplace* DestinationWorkplace;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	EBuildingClass CarClass;
	
	UMaterialInterface* RedMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Red.HouseColour_Red'"));
	UMaterialInterface* BlueMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Blue.HouseColour_Blue'"));
	UMaterialInterface* GreenMAT = LoadObject<UMaterialInterface>(NULL, TEXT("/Script/Engine.Material'/Game/GameObjects/Assets/House/Asset/HouseColour_Green.HouseColour_Green'"));

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
