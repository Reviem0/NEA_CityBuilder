// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"

#include "CB_CameraControl.generated.h"

UCLASS()
class NEA2_API ACB_CameraControl : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACB_CameraControl();

	UFUNCTION()
	void RotationTimelineProgress(float Value);
	FTimeline RotationCurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* RotationCurveFloat;

	UFUNCTION()
	void ViewTimelineProgress(float Value);
	FTimeline ViewCurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* ViewCurveFloat;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	bool isTurning = false;

	UPROPERTY()
	bool TopDown = false;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Camera control components
	UPROPERTY()
	UCameraComponent* CamControl; 

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY()
	FRotator CurrentArmRotation;

	UPROPERTY(EditAnywhere)
	FRotator TargetArmRotation;

	UPROPERTY()
	float CurrentArmPitch;

	UPROPERTY()
	float TargetArmPitch;

	UPROPERTY()
	float CurrentArmDistance;

	UPROPERTY()
	float TargetArmDistance;

	UPROPERTY(EditAnywhere)
	float TurnSens = 90.f;


	UPROPERTY()
	float StartLoc = 0.f;
	UPROPERTY()
	float EndLoc = 1.f;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	float ZOffset;

	// Input handlers
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void TurnRight();
	void TurnLeft();
	void ChangeView();
};