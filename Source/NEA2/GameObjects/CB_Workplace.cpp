// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_Workplace.h"

ACB_Workplace::ACB_Workplace() 
{
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::None;
    BuildingClass = EBuildingClass::Red;
}

// Sets default values
void ACB_Workplace::BeginPlay()
{
    Super::BeginPlay();
    
    if (!GridCellRef) {
        UE_LOG(LogTemp, Warning, TEXT("WORKPLACE: GridCellRef is null"));
        Destroy();
        return;
    }
    int Orientation = FMath::RandRange(0, 3);
    //UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Orientation: %d"), Orientation);

    AGridCell* BottomLeft = GridCellRef;
    AGridCell* BottomRight = GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
    AGridCell* TopLeft = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
    AGridCell* TopRight = BottomRight && BottomRight->NNeighbour ? *(BottomRight->NNeighbour) : nullptr;
    
    AGridCell* RoadPlacement = BottomLeft->SNeighbour ? *(BottomLeft->SNeighbour) : nullptr;
    FRotator* Rotation = new FRotator(0,0,0);

    Orientation = 3;

    switch (Orientation)
    {
        case 0:
        {
            UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Orientation: 0"));
            BottomLeft = GridCellRef;
            BottomRight = GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
            TopLeft = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
            TopRight = BottomRight && BottomRight->NNeighbour ? *(BottomRight->NNeighbour) : nullptr;
    
            RoadPlacement = BottomLeft->SNeighbour ? *(BottomLeft->SNeighbour) : nullptr;
            Rotation = new FRotator(0,0,0);
            break;
        }
        case 1:
        {
            UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Orientation: 1"));
            BottomLeft = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
            BottomRight = GridCellRef;
            TopRight = GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
            TopLeft = BottomLeft && BottomLeft->ENeighbour ? *(BottomLeft->ENeighbour) : nullptr;

            RoadPlacement = BottomLeft->WNeighbour ? *(BottomLeft->WNeighbour) : nullptr;
            Rotation = new FRotator(0,90,0);
            break;
        }
        case 2:
        {
            UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Orientation: 2"));
            BottomLeft = GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
            BottomRight = BottomLeft && BottomLeft->NNeighbour ? *(BottomLeft->NNeighbour) : nullptr;
            TopRight = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
            TopLeft = GridCellRef;

            RoadPlacement = BottomLeft->ENeighbour ? *(BottomLeft->ENeighbour) : nullptr;
            Rotation = new FRotator(0,270,0);
            break;
        }
        case 3:
        {
            UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Orientation: 3"));
            BottomRight = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
            TopRight = GridCellRef;
            TopLeft = GridCellRef && GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
            BottomLeft = BottomRight && BottomRight->ENeighbour ? *(BottomRight->ENeighbour) : nullptr;

            RoadPlacement = BottomLeft && BottomLeft->NNeighbour ? *(BottomLeft->NNeighbour) : nullptr;
            Rotation = new FRotator(0,180,0);
            break;
        }
    }

    // Create an FRotator from Rotation
    FRotator Rot(0, Rotation->Yaw, 0);

    

    if ((!BottomLeft || !TopRight || !TopLeft || !BottomRight || !RoadPlacement) || (BottomLeft->OccupyingType != EBuildingType::None || TopRight->OccupyingType != EBuildingType::None || TopLeft->OccupyingType != EBuildingType::None || BottomRight->OccupyingType != EBuildingType::None || RoadPlacement->OccupyingType != EBuildingType::None))
    {
        UE_LOG(LogTemp, Warning, TEXT("WORKPLACE: NOT ENOUGH CELLS"));
        Destroy();
        return;
    }
    
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    if (BottomLeftActor)
    {
        BottomLeftAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(BottomLeftActor, BottomLeft->GetActorLocation(), Rot, SpawnInfo);
        if (BottomLeftAsset) {
            BottomLeft->Manager = this;
            BottomLeftAsset->WorkplaceRef = this;
        }
    }

    if (TopRightActor)
    {
        TopRightAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(TopRightActor, TopRight->GetActorLocation(), Rot, SpawnInfo);
        if (TopRightAsset) {
            TopRight->Manager = this;
            TopRightAsset->WorkplaceRef = this;
        }
    }

    if (TopLeftActor)
    {
        TopLeftAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(TopLeftActor, TopLeft->GetActorLocation(), Rot, SpawnInfo);
        if (TopLeftAsset) {
            TopLeft->Manager = this;
            TopLeftAsset->WorkplaceRef = this;
        }
    }

    if (BottomRightActor)
    {
        BottomRightAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(BottomRightActor, BottomRight->GetActorLocation(), Rot, SpawnInfo);
        if (BottomRightAsset) {
            BottomRight->Manager = this;
            BottomRightAsset->WorkplaceRef = this;
        }
    }
    if (RoadTileActor)
    {   
        RoadPlacement->Manager = this;
        RoadTileAsset = GetWorld()->SpawnActor<ACB_OwnedRoadCell>(RoadTileActor, RoadPlacement->GetActorLocation(), Rot, SpawnInfo);
        if (RoadTileAsset) {
            RoadTileAsset->OwningCell = BottomLeft;
        }
    }
}

// Called every frame
void ACB_Workplace::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
