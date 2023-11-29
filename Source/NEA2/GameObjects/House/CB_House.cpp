// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_House.h"



// Sets default values
ACB_House::ACB_House()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::House;
}

// Called when the game starts or when spawned
void ACB_House::BeginPlay()
{
    Super::BeginPlay();
    // Generate random number between 0 and 3
    int32 RandomNumber = FMath::RandRange(0, 3);
    // Set rotation based on random number
    if (RandomNumber == 0) SetActorRotation(FRotator(0, 0, 0));
    if (RandomNumber == 1) SetActorRotation(FRotator(0, 90, 0));
    if (RandomNumber == 2) SetActorRotation(FRotator(0, 180, 0));
    if (RandomNumber == 3) SetActorRotation(FRotator(0, 270, 0));

    BuildingClass = EBuildingClass::Red;
    
}

// Called every frame
void ACB_House::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}



