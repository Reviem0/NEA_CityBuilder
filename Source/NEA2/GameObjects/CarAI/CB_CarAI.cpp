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
	// Advance the timeline every frame
	MovementTimeline.TickTimeline(DeltaTime);

}

void ACB_CarAI::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// If the car is already colliding with this car, return
	if (Cast<ACB_CarAI>(OtherActor)->CollidingCar == this){
		return;
	}
	// If the car is not already colliding and the other actor is not itself
	if (Overlapping == false && OtherActor != this){
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
		// Reset the colliding car to null
		CollidingCar = nullptr;
		// Set delay to 0.5 seconds
		// Play the car after a 0.5 second delay
		FTimerHandle Delay;
		GetWorldTimerManager().SetTimer(Delay, this, &ACB_CarAI::PlayCar, 0.5f, false);
	}
}

void ACB_CarAI::FollowSpline(USplineComponent* Spline)
{
	CarClass = OriginHouse->BuildingClass;
	// Set Material based on class
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);

	// Set the material based on the class
	for (UStaticMeshComponent* MeshComponent : MeshComponents) {
		switch (CarClass)
		{
			// If the car is red
			case EBuildingClass::Red:
				MeshComponent->SetMaterial(0, RedMAT);
				break;
			// If the car is blue
			case EBuildingClass::Blue:
				MeshComponent->SetMaterial(0, BlueMAT);
				break;
			// If the car is green
			case EBuildingClass::Green:
				MeshComponent->SetMaterial(0, GreenMAT);
				break;
		}
	}
	// Set the spline to follow
	SplineToFollow = Spline;

	// Set the speed of the car
	float Playrate = (1/ SplineToFollow->GetSplineLength()) * 200;
	MovementTimeline.SetPlayRate(Playrate);

	// Play the timeline
    MovementTimeline.PlayFromStart();
}

void ACB_CarAI::TimelineFloatReturn(float value)
{
    if (SplineToFollow)
    {
		// Get the transform of the spline and apply it to the car
        FTransform NewTransform = SplineToFollow->GetTransformAtDistanceAlongSpline(value * SplineToFollow->GetSplineLength(), ESplineCoordinateSpace::World);
        SetActorTransform(NewTransform);
    }
}

// Called when the car has reached the end of the spline
void ACB_CarAI::OnTimelineFinished()
{
	// Log that the timeline has finished for debugging
	UE_LOG(LogTemp, Warning, TEXT("OnTimelineFinished called for %s"), *GetName());

	// If the car is not returning home
	if (!Returning){
		// Call the car arrived function in the destination workplace
		DestinationWorkplace->CarArrived(this);
	} else {
		// Otherwise, all the car arrived function in the origin house
		Cast<ACB_House>(OriginHouse)->CarArrived(this);
	}
	
	// Destroy the car after it has finished its journey
	Destroy();
}

void ACB_CarAI::PlayCar()
{
	UE_LOG(LogTemp, Display, TEXT("Overlap End"));
	// Continue the car
	MovementTimeline.Play();
}