// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_BuildingAsset.h"
#include "../Character/CB_PlayerController.h"
#include "GameFramework/PlayerController.h"


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
	// check if ploppable
	ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController->PlaceableActor == this || PlayerController->PlaceableActor == nullptr) {
		isPlop = true;
	} else
	{
		isPlop = false;
	}
	
}

// Called every frame
void ACB_BuildingAsset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	

}

