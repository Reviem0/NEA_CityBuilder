// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_BuildingAsset.h"


// Sets default values
ACB_BuildingAsset::ACB_BuildingAsset()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Set Collision preset to buildings
	

}

// Called when the game starts or when spawned
void ACB_BuildingAsset::BeginPlay()
{
	Super::BeginPlay();

	// add 2 +2 

	
}

// Called every frame
void ACB_BuildingAsset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCB_PloppableComponent* PlopComp = GetComponentByClass<UCB_PloppableComponent>();
	if (PlopComp) {
		isPlop = true;
	} else
	{
		isPlop = false;
	}

}

