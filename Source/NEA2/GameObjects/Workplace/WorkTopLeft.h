// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_WorkplaceAsset.h"
#include "WorkTopLeft.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API AWorkTopLeft : public ACB_WorkplaceAsset
{
	GENERATED_BODY()
	
public:
	AWorkTopLeft();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
