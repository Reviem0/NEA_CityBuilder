// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

UCLASS()
class NEA2_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();
	
	// Grid Default Settings
	UPROPERTY(EditAnywhere)
	int32 GridSize = 10;
	UPROPERTY(EditAnywhere)
	int32 WorldGridSize = 1000;
	UPROPERTY(EditAnywhere)
	float HeightOffset = 0;
	UPROPERTY()
	float WorldOffset;

	UPROPERTY(EditAnywhere)
	float objsize = 1;


	UFUNCTION()
	void PopulateGrid();

	UPROPERTY(EditDefaultsOnly,Category="ActorSpawning")
	TSubclassOf<AGrid> AGridBP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<AActor*> GridArray;

};
