// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_CarAI.h"
#include "../House/CB_House.h"
#include "Components/StaticMeshComponent.h"

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

		FOnTimelineEvent TimelineFinished;
        TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
        MovementTimeline.SetTimelineFinishedFunc(TimelineFinished);
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
	CarClass = Cast<ACB_House>(OriginHouse)->BuildingClass;
	// Set Material based on class
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);
	for (UStaticMeshComponent* MeshComponent : MeshComponents) {
		switch (CarClass)
		{
			case EBuildingClass::Red:
				MeshComponent->SetMaterial(0, RedMAT);
				break;
			case EBuildingClass::Blue:
				MeshComponent->SetMaterial(0, BlueMAT);
				break;
			case EBuildingClass::Green:
				MeshComponent->SetMaterial(0, GreenMAT);
				break;
		}
	}

	SplineToFollow = Spline;
	float Playrate = (1/ SplineToFollow->GetSplineLength()) * 200;
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

void ACB_CarAI::OnTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("OnTimelineFinished called for %s"), *GetName());
	if (!Returning){
		DestinationWorkplace->CarArrived(this);
	} else {
		Cast<ACB_House>(OriginHouse)->CarArrived(this);
	}
	
	Destroy();
}


