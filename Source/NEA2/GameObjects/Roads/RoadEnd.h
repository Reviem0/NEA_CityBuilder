// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../CB_RoadTile.h"
#include "RoadEnd.generated.h"

UCLASS()
class NEA2_API ARoadEnd : public ACB_RoadTile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoadEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
