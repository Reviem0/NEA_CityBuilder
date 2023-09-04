// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CB_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NEA2_API ACB_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACB_PlayerController();

	AActor* PlaceableActor;
	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
