// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_PlayerController.h"
#include "Kismet/GameplayStatics.h"


ACB_PlayerController::ACB_PlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACB_PlayerController::BeginPlay()
{
    SetInputMode(FInputModeGameAndUI());
    RoadInventory = 20;

}

void ACB_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Place", IE_Pressed, this, &ACB_PlayerController::SpawnBuilding);
    InputComponent->BindAction("Remove", IE_Pressed, this, &ACB_PlayerController::StartDeletingBuilding);
    InputComponent->BindAction("Remove", IE_Released, this, &ACB_PlayerController::StopDeletingBuilding);
    InputComponent->BindAction("PathTEST", IE_Pressed, this, &ACB_PlayerController::PathTEST);
    InputComponent->BindAction("IncreaseSpeed", IE_Pressed, this, &ACB_PlayerController::IncreaseSpeed);
    InputComponent->BindAction("IncreaseSpeed", IE_Released, this, &ACB_PlayerController::ResetSpeed);
    InputComponent->BindAction("EnablePlacement", IE_Pressed, this, &ACB_PlayerController::PlacementCheck);
    InputComponent->BindAction("ForceLoss", IE_Pressed, this, &ACB_PlayerController::ForceLoss);

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
        SetPlacementModeEnabled(!PlacementModeEnabled);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("GridManager not found! Cannot place buildings!"));
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
        PlaceableActor = GetWorld()->SpawnActorDeferred<ACB_BuildingAsset>(ActorToPlace, RelativeTransform);
        if (PlaceableActor) {
            PlaceableActor->SetActorScale3D(GridManager->GetGridScale());
            if (PlaceableActor->GetComponentByClass<UCB_ClickComponent>()){
                PlaceableActor->GetComponentByClass<UCB_ClickComponent>()->inPlacementMode();
                FName CompName = "Ploppable";
            }
            NewComponent = NewObject<UCB_PloppableComponent>(PlaceableActor);
            if (NewComponent){
                NewComponent->RegisterComponent();
                PlaceableActor->AddInstanceComponent(NewComponent);
            }
            PlaceableActor->isPlop = true;
            PlaceableActor->FinishSpawning(RelativeTransform);
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
    if (PlacementModeEnabled == false) {
        return;
    }
    if (RoadInventory <= 0) {
        UE_LOG(LogTemp, Warning, TEXT("Road Inventory is empty!"));
        return;
    }
    UCB_PloppableComponent* PlopComp = PlaceableActor->GetComponentByClass<UCB_PloppableComponent>();
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (PlopComp->IsPlacementValid) {
        AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorToPlace, PlaceableActor->GetActorTransform(), SpawnParams);
        PlopComp->UpdateState();
        LastPlaced = PlaceableActor->GridCellRef;
        if (GridManager->GameManager) {
            GridManager->GameManager->UpdatePath();
        }
        UpdateRoadInventory(-1);
    }
}

void ACB_PlayerController::DeleteBuilding() {
    if (PlacementModeEnabled == false) {
        return;
    }
    if (PlaceableActor) {
        if (GridManager){
            UCB_PloppableComponent* PlopComp = PlaceableActor->GetComponentByClass<UCB_PloppableComponent>();
            AGridCell* Grid = GridManager->GetClosestGridCell(PlaceableActor->GetActorLocation());
            if (Grid->OccupyingActor){
                if (Cast<ACB_BuildingAsset>(Grid->OccupyingActor) && Grid->OccupyingType == EBuildingType::Road && Cast<ACB_BuildingAsset>(Grid->OccupyingActor)->Ownership == EOwnership::Player)
                {
                    Cast<ACB_RoadCell>(Grid->OccupyingActor)->DestroyRoad();
                    PlopComp->UpdateState();
                    if (GridManager->GameManager) { 
                        GridManager->GameManager->UpdatePath();
                    }
                    UpdateRoadInventory(1);
                }
            }
        }
    }
}

void ACB_PlayerController::StartDeletingBuilding()
{
    DeleteBuilding();
    if (!bDeletingBuilding)
    {
        bDeletingBuilding = true;
        GetWorldTimerManager().SetTimer(DeleteBuildingTimerHandle, this, &ACB_PlayerController::DeleteBuilding, DeleteBuildingInterval, true);
    }
}

void ACB_PlayerController::StopDeletingBuilding()
{
    if (bDeletingBuilding)
    {
        bDeletingBuilding = false;
        GetWorldTimerManager().ClearTimer(DeleteBuildingTimerHandle);
    }
}

void ACB_PlayerController::PathTEST() {
    if (GridManager){
        for (int i = 0; i < GridManager->GridArray.Num(); i++) {
            GridManager->GridArray[i]->ResetMAT();
        }
        AGridCell* StartCell = LastPlaced;
        AGridCell* EndCell = GridManager->GridArray[0];
        TArray<AGridCell*> Path = GridManager->FindPath(StartCell, EndCell);
        for (int i = 0; i < Path.Num(); i++) {
		    Path[i]->DebugSetMAT();
	    }
    }
}

void ACB_PlayerController::IncreaseSpeed() {
    // set global time dilation to 2
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), FastForward);
}

void ACB_PlayerController::ResetSpeed() {
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

void ACB_PlayerController::UpdateRoadInventory(int Amount) {
    RoadInventory += Amount;
    if (PlaceableActor) {
        if (PlaceableActor->GetComponentByClass<UCB_PloppableComponent>()) {
            PlaceableActor->GetComponentByClass<UCB_PloppableComponent>()->UpdateState();
        }
    }
}

void ACB_PlayerController::ForceLoss() {
    if (GridManager) {
        GridManager->GameManager->LossFunction();
    }
}