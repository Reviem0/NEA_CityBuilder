// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_CarAI.h"
#include "../House/CB_House.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values 

ACB_CarAI::ACB_CarAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create the root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Create the mesh
	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	CarMesh->SetupAttachment(RootComponent);
	
	// Define the trigger box
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);

	// Change the size of the trigger box
	TriggerBox->SetBoxExtent(FVector(14, 4, 2));

}

// Called when the game starts or when spawned
void ACB_CarAI::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox) {
		// Bind overlap events
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACB_CarAI::OnOverlapBegin);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ACB_CarAI::OnOverlapEnd);
	}

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

void ACB_CarAI::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (Cast<ACB_CarAI>(OtherActor)->CollidingCar == this){
		return;
	}
	if (Overlapping == false && Cast<ACB_CarAI>(OtherActor)->Returning == this->Returning && OtherActor != this){
		Overlapping = true;
		// Set the colliding car
		CollidingCar = Cast<ACB_CarAI>(OtherActor);
		// Log what the car overlapped with
		UE_LOG(LogTemp, Display, TEXT("Overlap Begin with %s"), *OtherActor->GetName());
		// Stop the car
		MovementTimeline.Stop();
	}

}

void ACB_CarAI::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (Overlapping == true){
		Overlapping = false;
		CollidingCar = nullptr;
		// Set delay to 0.5 seconds
		FTimerHandle Delay;
		GetWorldTimerManager().SetTimer(Delay, this, &ACB_CarAI::PlayCar, 0.5f, false);
	}
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

void ACB_CarAI::PlayCar()
{
	UE_LOG(LogTemp, Display, TEXT("Overlap End"));
	// Continue the car
	MovementTimeline.Play();
}