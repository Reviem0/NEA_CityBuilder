// Fill out your copyright notice in the Description page of Project Settings. 

#include "CB_CameraControl.h"
#include "Components/TimelineComponent.h"


// Sets default values
ACB_CameraControl::ACB_CameraControl()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = false;
	

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;


}

// Called when the game starts or when spawned
void ACB_CameraControl::BeginPlay() 
{
	Super::BeginPlay();
	
	// Initialize CamControl and CurrentCamRotation
	CamControl = Camera; 
	SpringArm = SpringArmComponent;
	CurrentArmRotation = FRotator::ZeroRotator;

  if(CurveFloat) 
  {
    FOnTimelineFloat TimelineProgress;
    TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
    CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
  }
}

// Called every frame
void ACB_CameraControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  CurveTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ACB_CameraControl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACB_CameraControl::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACB_CameraControl::MoveRight);
	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &ACB_CameraControl::TurnRight);
	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &ACB_CameraControl::TurnLeft);
}

void ACB_CameraControl::MoveForward(float Axis)
{
	AddMovementInput(SpringArm->GetForwardVector()* Axis);
}

void ACB_CameraControl::MoveRight(float Axis)
{
	AddMovementInput(SpringArm->GetRightVector()* Axis);  
}

void ACB_CameraControl::TurnRight() 
{
  if (CamControl)  
  {
    if ((CurveTimeline.IsPlaying())) {
      return;
    }
    else {
      isTurning = true;
      CurrentArmRotation = SpringArm->GetComponentRotation();
      TargetArmRotation = FRotator(CurrentArmRotation.Pitch, CurrentArmRotation.Yaw + TurnSens, CurrentArmRotation.Roll); 
      CurveTimeline.PlayFromStart();
    }
  }
}

void ACB_CameraControl::TurnLeft()
{
  if (CamControl)  
  {
    if (CurveTimeline.IsPlaying()) {
      return;
    }
    else {
      isTurning = true;
      CurrentArmRotation = SpringArm->GetComponentRotation();
      TargetArmRotation = FRotator(CurrentArmRotation.Pitch, CurrentArmRotation.Yaw - TurnSens, CurrentArmRotation.Roll); 
      CurveTimeline.PlayFromStart();
    }
  }
}


void ACB_CameraControl::TimelineProgress(float Alpha) 
{
  FRotator NewArmRotation = FMath::Lerp(CurrentArmRotation, TargetArmRotation, Alpha);
  SpringArm->SetRelativeRotation(NewArmRotation);
}