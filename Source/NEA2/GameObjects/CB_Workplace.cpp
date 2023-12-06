// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_Workplace.h"

ACB_Workplace::ACB_Workplace() 
{
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::Workplace;
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

    AGridCell* BottomLeft = GridCellRef;
    AGridCell* BottomRight = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
    AGridCell* TopLeft = GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
    AGridCell* TopRight = BottomRight && BottomRight->ENeighbour ? *(BottomRight->ENeighbour) : nullptr;
    
    AGridCell* RoadPlacement = BottomLeft->SNeighbour ? *(BottomLeft->SNeighbour) : nullptr;

    if (!BottomLeft || !TopRight || !TopLeft || !BottomRight || !RoadPlacement)
    {
        UE_LOG(LogTemp, Warning, TEXT("WORKPLACE: NOT ENOUGH CELLS"));
        Destroy();
        return;
    }
    
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    if (BottomLeftActor)
    {
        BottomLeftAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(BottomLeftActor, BottomLeft->GetActorLocation(), BottomLeft->GetActorRotation(), SpawnInfo);
        if (BottomLeftAsset) {
            BottomLeft->Manager = this;
            BottomLeftAsset->WorkplaceRef = this;
        }
    }

    if (TopRightActor)
    {
        TopRightAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(TopRightActor, TopRight->GetActorLocation(), TopRight->GetActorRotation(), SpawnInfo);
        if (TopRightAsset) {
            TopRight->Manager = this;
            TopRightAsset->WorkplaceRef = this;
        }
    }

    if (TopLeftActor)
    {
        TopLeftAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(TopLeftActor, TopLeft->GetActorLocation(), TopLeft->GetActorRotation(), SpawnInfo);
        if (TopLeftAsset) {
            TopLeft->Manager = this;
            TopLeftAsset->WorkplaceRef = this;
        }
    }

    if (BottomRightActor)
    {
        BottomRightAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(BottomRightActor, BottomRight->GetActorLocation(), BottomRight->GetActorRotation(), SpawnInfo);
        if (BottomRightAsset) {
            BottomRight->Manager = this;
            BottomRightAsset->WorkplaceRef = this;
        }
    }
    if (RoadTileActor)
    {   
        RoadPlacement->Manager = this;
        RoadTileAsset = GetWorld()->SpawnActor<ACB_OwnedRoadTile>(RoadTileActor, RoadPlacement->GetActorLocation(), RoadPlacement->GetActorRotation(), SpawnInfo);
        if (RoadTileAsset) {
            
            RoadTileAsset->Owner = BottomLeftAsset;
        }
    }
}

// Called every frame
void ACB_Workplace::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
