// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_PlayerController.h"
#include "Kismet/GameplayStatics.h"

ACB_PlayerController::ACB_PlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACB_PlayerController::BeginPlay()
{
    SetPlacementModeEnabled(true);


}

void ACB_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Place", IE_Pressed, this, &ACB_PlayerController::SpawnBuilding);
}

void ACB_PlayerController::Tick(float DeltaTime)
{
    if (PlacementModeEnabled) {
        UpdatePlacement();
    }
}

void ACB_PlayerController::SetPlacementModeEnabled(bool bEnabled) {
    if (PlacementModeEnabled == bEnabled) {
        return;
    }
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FTransform RelativeTransform;
    RelativeTransform.SetLocation(FVector(0,0,-10000));

    PlacementModeEnabled = bEnabled;
    if (!PlacementModeEnabled) {
        if (PlaceableActor) {
            PlaceableActor->Destroy();
        }
    } else {
        PlaceableActor = GetWorld()->SpawnActor<ACB_BuildingAsset>(ActorToPlace, RelativeTransform, SpawnParams);
        if (PlaceableActor->GetComponentByClass<UCB_ClickComponent>()){
            PlaceableActor->GetComponentByClass<UCB_ClickComponent>()->inPlacementMode();
            FName CompName = "Ploppable";
            UCB_PloppableComponent* NewComponent = NewObject<UCB_PloppableComponent>(PlaceableActor);
            if (NewComponent){
                PlaceableActor->AddInstanceComponent(NewComponent);
                NewComponent->RegisterComponent();
            }


        }
    }
}

void ACB_PlayerController::UpdatePlacement() {
    FVector WorldLocation;
    FVector WorldDirection;
    FHitResult Hit;
    DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
    bool HitResult = GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, WorldLocation + WorldDirection * 7000, ECC_GameTraceChannel1);

    if (HitResult){
        AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
        if (GridManager){
            GridManager->GetClosestGridPosition(Hit.Location);
            PlaceableActor->SetActorLocation(GridManager->GetClosestGridPosition(Hit.Location));
        }
    }
}

void ACB_PlayerController::SpawnBuilding() {
    UCB_PloppableComponent* PlopComp = PlaceableActor->GetComponentByClass<UCB_PloppableComponent>();
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (PlopComp) {
        if (PlopComp->IsPlacementValid) {
            ACB_BuildingAsset* NewActor = GetWorld()->SpawnActor<ACB_BuildingAsset>(ActorToPlace, PlaceableActor->GetActorTransform(), SpawnParams);
        }
    }
}
