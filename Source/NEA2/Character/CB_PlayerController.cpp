// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_PlayerController.h"
#include "Kismet/GameplayStatics.h"

ACB_PlayerController::ACB_PlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACB_PlayerController::BeginPlay()
{
    PlacementCheck();
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

void ACB_PlayerController::PlacementCheck() 
{
    GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    if (GridManager){
        SetPlacementModeEnabled(true);
    } else {
        PlacementCheck();
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
        PlaceableActor->SetActorScale3D(GridManager->GetGridScale());
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
        if (GridManager){
            AGridCell* GridCell = GridManager->GetClosestGridCell(Hit.Location);
            FVector Spawnpoint = GridCell->GetActorLocation();
            PlaceableActor->SetActorLocation(Spawnpoint);
            PlaceableActor->GridCellRef = GridCell;
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
            NewActor->GridCellRef = PlaceableActor->GridCellRef;
            NewActor->SetActorScale3D(GridManager->GetGridScale());
            NewActor->GetComponentByClass<UCB_PloppableComponent>()->DestroyComponent();
        }
    }
}
