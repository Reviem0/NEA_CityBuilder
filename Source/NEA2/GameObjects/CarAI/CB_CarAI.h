// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"

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

    UFUNCTION()
    void TimelineFloatReturn(float value);

    UPROPERTY()
    USplineComponent* SplineToFollow;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
