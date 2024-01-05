// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_CarAI.h"

// Sets default values
ACB_CarAI::ACB_CarAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACB_CarAI::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineFloatReturn"));
		MovementTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
	
}

// Called every frame
void ACB_CarAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovementTimeline.TickTimeline(DeltaTime);

}

void ACB_CarAI::FollowSpline(USplineComponent* Spline)
{
	SplineToFollow = Spline;
	float Playrate = 1 / SplineToFollow->GetSplineLength();
	MovementTimeline.SetPlayRate(Playrate);
    MovementTimeline.PlayFromStart();
}

void ACB_CarAI::TimelineFloatReturn(float value)
{
    if (SplineToFollow)
    {
        FTransform NewTransform = SplineToFollow->GetTransformAtDistanceAlongSpline(value * SplineToFollow->GetSplineLength(), ESplineCoordinateSpace::World);
        SetActorTransform(NewTransform);
    }
}



