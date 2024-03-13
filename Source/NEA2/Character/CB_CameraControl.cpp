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
  SpringArmComponent->SetWorldRotation(FRotator(330,0,0));
	

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

  if(RotationCurveFloat) 
  {
    FOnTimelineFloat RotationTimelineProgress;
    RotationTimelineProgress.BindUFunction(this, FName("RotationTimelineProgress"));
    RotationCurveTimeline.AddInterpFloat(RotationCurveFloat, RotationTimelineProgress);
  }

  if(ViewCurveFloat) 
  {
    FOnTimelineFloat ViewTimelineProgress;
    ViewTimelineProgress.BindUFunction(this, FName("ViewTimelineProgress"));
    ViewCurveTimeline.AddInterpFloat(ViewCurveFloat, ViewTimelineProgress);
  }
}

// Called every frame
void ACB_CameraControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  RotationCurveTimeline.TickTimeline(DeltaTime);
  ViewCurveTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ACB_CameraControl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACB_CameraControl::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACB_CameraControl::MoveRight);
	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &ACB_CameraControl::TurnRight);
	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &ACB_CameraControl::TurnLeft);
  PlayerInputComponent->BindAction("ChangeView", IE_Pressed, this, &ACB_CameraControl::ChangeView);
}

void ACB_CameraControl::MoveForward(float Axis)
{
  AddMovementInput(SpringArm->GetForwardVector()* Axis);
}

void ACB_CameraControl::MoveRight(float Axis)
{
  if (!TopDown){
	  AddMovementInput(SpringArm->GetRightVector()* Axis);
  }
}

void ACB_CameraControl::TurnRight() 
{
  if (CamControl)  
  {
    if ((RotationCurveTimeline.IsPlaying())) {
      return;
    }
    else {
      isTurning = true;
      CurrentArmRotation = SpringArm->GetComponentRotation();
      TargetArmRotation = FRotator(CurrentArmRotation.Pitch, CurrentArmRotation.Yaw + TurnSens, CurrentArmRotation.Roll); 
      RotationCurveTimeline.PlayFromStart();
    }
  }
}

void ACB_CameraControl::TurnLeft()
{
  if (CamControl)  
  {
    if (RotationCurveTimeline.IsPlaying()) {
      return;
    }
    else {
      isTurning = true;
      CurrentArmRotation = SpringArm->GetComponentRotation();
      TargetArmRotation = FRotator(CurrentArmRotation.Pitch, CurrentArmRotation.Yaw - TurnSens, CurrentArmRotation.Roll); 
      RotationCurveTimeline.PlayFromStart();
    }
  }
}

void ACB_CameraControl::ChangeView()
{
  UE_LOG(LogTemp, Display, TEXT("ChangeView"));
  if (CamControl)  
  {
    if (RotationCurveTimeline.IsPlaying() || ViewCurveTimeline.IsPlaying()) {
      return;
    }
    if (!TopDown){
      TopDown = true;
      isTurning = true;
      CurrentArmRotation = SpringArm->GetComponentRotation();
      CurrentArmPitch = CurrentArmRotation.Pitch;
      CurrentArmDistance = SpringArm->TargetArmLength;
      TargetArmDistance = 1000.f;
      TargetArmPitch = -90.f;
      ViewCurveTimeline.PlayFromStart();
    } else {
      TopDown = false;
      isTurning = true;
      CurrentArmRotation = SpringArm->GetRelativeRotation();
      CurrentArmPitch = CurrentArmRotation.Pitch;
      TargetArmPitch = -30.f;
      CurrentArmDistance = SpringArm->TargetArmLength;
      TargetArmDistance = 300.f;
      ViewCurveTimeline.PlayFromStart();
    }
  }
}

void ACB_CameraControl::RotationTimelineProgress(float Alpha) 
{
  FRotator NewArmRotation = FMath::Lerp(CurrentArmRotation, TargetArmRotation, Alpha);
  SpringArm->SetRelativeRotation(NewArmRotation);
}

void ACB_CameraControl::ViewTimelineProgress(float Value) 
{
  float NewArmPitch = FMath::Lerp(CurrentArmPitch, TargetArmPitch, Value);
  float NewArmDistance = FMath::Lerp(CurrentArmDistance, TargetArmDistance, Value);
  //UE_LOG(LogTemp, Display, TEXT("New Rotation: %f, %f, %f"), NewArmPitch, CurrentArmRotation.Yaw, CurrentArmRotation.Roll);
  
  SpringArm->SetRelativeRotation(FRotator(NewArmPitch, CurrentArmRotation.Yaw, CurrentArmRotation.Roll));
  SpringArm->TargetArmLength = NewArmDistance;
}
