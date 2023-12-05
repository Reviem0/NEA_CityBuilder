// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGameManager::Init()
{
	int RandomNumber1 = FMath::RandRange(0, GridArray.Num()-1);
	int RandomNumber2 = FMath::RandRange(0, GridArray.Num()-1);
	SpawnHouse(GridArray[RandomNumber1]);
	SpawnWorkplace(GridArray[RandomNumber2]);
	
}

void AGameManager::SpawnHouse(AGridCell* GridCell) 
{
	if (HouseClass == nullptr) return;

	ACB_House* House = GetWorld()->SpawnActor<ACB_House>(HouseClass, GridCell->GetActorLocation(), FRotator::ZeroRotator);
	
}

void AGameManager::SpawnWorkplace(AGridCell* GridCell)
{
	if (WorkplaceClass == nullptr) return;

	ACB_Workplace* Workplace = GetWorld()->SpawnActor<ACB_Workplace>(WorkplaceClass, GridCell->GetActorLocation(), FRotator::ZeroRotator);

}