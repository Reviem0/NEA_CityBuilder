// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObjects/CB_Workplace.h"
#include "GameObjects/House/CB_House.h"

#include "GameManager.generated.h"

UCLASS()
class NEA2_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	TArray<AGridCell*> GridArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACB_House> HouseClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACB_Workplace> WorkplaceClass;
	void SpawnHouse(AGridCell* GridCell);
	void SpawnWorkplace(AGridCell* GridCell);

	void Init();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
