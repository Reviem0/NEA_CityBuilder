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

    if (!BottomLeft || !TopRight || !TopLeft || !BottomRight)
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
            BottomLeftAsset->WorkplaceRef = this;
        }
    }

    if (TopRightActor)
    {
        TopRightAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(TopRightActor, TopRight->GetActorLocation(), TopRight->GetActorRotation(), SpawnInfo);
        if (TopRightAsset) {
            TopRightAsset->WorkplaceRef = this;
        }
    }

    if (TopLeftActor)
    {
        TopLeftAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(TopLeftActor, TopLeft->GetActorLocation(), TopLeft->GetActorRotation(), SpawnInfo);
        if (TopLeftAsset) {
            TopLeftAsset->WorkplaceRef = this;
        }
    }

    if (BottomRightActor)
    {
        BottomRightAsset = GetWorld()->SpawnActor<ACB_WorkplaceAsset>(BottomRightActor, BottomRight->GetActorLocation(), BottomRight->GetActorRotation(), SpawnInfo);
        if (BottomRightAsset) {
            BottomRightAsset->WorkplaceRef = this;
        }
    }
}

// Called every frame
void ACB_Workplace::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
