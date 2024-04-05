// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_RoadCell.h"
#include "../Grid/GridManager.h"
#include "Kismet/GameplayStatics.h"

ACB_RoadCell::ACB_RoadCell() {
    PrimaryActorTick.bCanEverTick = true;

    BuildingType = EBuildingType::Road;
    Ownership = EOwnership::Player;

}

void ACB_RoadCell::BeginPlay() {
    Super::BeginPlay();

    if (GridCellRef)
    {
        OldNorthR = false;
        OldSouthR = false;
        OldEastR = false;
        OldWestR = false;
    }

    if (GridCellRef && !isPlop) {
        // make actor invisible
        SetActorHiddenInGame(true);
        GridCellRef->SetOccupied(BuildingType, this);
        if (!isPlop)
        {
            UpdateRoadMesh();
        }
    }
}

void ACB_RoadCell::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACB_RoadCell::SpawnNewActor(UClass *ActorClass, FRotator Rotation)
{
    ACB_RoadTile* NewActor = GetWorld()->SpawnActorDeferred<ACB_RoadTile>(ActorClass, FTransform(Rotation, GetActorLocation(), FVector(1, 1, 1)));
    if (NewActor){
        NewActor->RoadCellOwner = this;
        UGameplayStatics::FinishSpawningActor(NewActor, FTransform(Rotation, GetActorLocation(), FVector(1, 1, 1)));
        if (RoadRef)
        {
            RoadRef->Destroy();
        }
        GridCellRef->SetOccupied(BuildingType, this);
        RoadRef = NewActor;
        UpdateNeighbours();
        IsUpdatingMesh = false;
    }
}

// Roads dont work because of the setOccupied system


void ACB_RoadCell::UpdateRoadMesh()
{
    IsUpdatingMesh = true;

    if (GridCellRef)
    {
        bool NorthR = GridCellRef->NNeighbour && ((*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road || ((*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::OwnedRoad));
        bool SouthR = GridCellRef->SNeighbour && ((*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road || ((*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::OwnedRoad));
        bool EastR = GridCellRef->ENeighbour && ((*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road || ((*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::OwnedRoad));
        bool WestR = GridCellRef->WNeighbour && ((*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road) || ((*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::OwnedRoad);

        int roadCount = NorthR + SouthR + EastR + WestR;

        if (NorthR == OldNorthR && SouthR == OldSouthR && EastR == OldEastR && WestR == OldWestR && (NorthR || SouthR || EastR || WestR))
        {
            IsUpdatingMesh = false;
            return;
        }

        OldNorthR = NorthR;
        OldSouthR = SouthR;
        OldEastR = EastR;
        OldWestR = WestR;


        switch (roadCount)
        {
            case 4:
            {
                SpawnNewActor(RoadCross, GetActorRotation());
                break;
            }
            case 3:
            {
                FRotator T_Rot = EastR && SouthR && WestR ? FRotator(0, 90, 0) : NorthR && SouthR && WestR ? FRotator(0, 180, 0) : NorthR && SouthR && EastR ? FRotator(0, 0, 0) : FRotator(0, 270, 0);
                SpawnNewActor(RoadT, T_Rot);
                break;
            }
            case 2:
            {
                if ((NorthR && SouthR || WestR && EastR))
                {
                    FRotator Straight_Rot = NorthR && SouthR ? FRotator(0, 90, 0) : FRotator(0, 0, 0);
                    SpawnNewActor(RoadStraight, Straight_Rot);
                }
                else
                {
                    FRotator Corner_Rot = NorthR && EastR ? FRotator(0, 90, 0) : NorthR && WestR ? FRotator(0,0,0) : SouthR && EastR ? FRotator(0, 180, 0) : SouthR && WestR ? FRotator(0, 270, 0) : FRotator(0, 0, 0);
                    SpawnNewActor(RoadCorner, Corner_Rot);
                }
                break;
            }
            case 1:
            {
                FRotator End_Rot = NorthR ? FRotator(0, 0, 0) : SouthR ? FRotator(0, 180, 0) : EastR ? FRotator(0, 90, 0) : FRotator(0, 270, 0);
                SpawnNewActor(RoadEnd, End_Rot);
                break;
            }
            case 0:
            {
                SpawnNewActor(RoadCircle, FRotator(0, 0, 0));
                break;
            }
        }
    }
    IsUpdatingMesh = false;
}

void ACB_RoadCell::UpdateNeighbours()
{
    if (GridCellRef)
    {
        if (GridCellRef->NNeighbour)
        {
            ACB_RoadCell* NorthNeighbour = Cast<ACB_RoadCell>((*(GridCellRef->NNeighbour))->OccupyingActor);
            if (NorthNeighbour && !NorthNeighbour->IsUpdatingMesh)
            {
                NorthNeighbour->UpdateRoadMesh();
            }
        }
        if (GridCellRef->SNeighbour)
        {
            ACB_RoadCell* SouthNeighbour = Cast<ACB_RoadCell>((*(GridCellRef->SNeighbour))->OccupyingActor);
            if (SouthNeighbour && !SouthNeighbour->IsUpdatingMesh)
            {
                SouthNeighbour->UpdateRoadMesh();
            }
        }
        if (GridCellRef->ENeighbour)
        {
            ACB_RoadCell* EastNeighbour = Cast<ACB_RoadCell>((*(GridCellRef->ENeighbour))->OccupyingActor);
            if (EastNeighbour && !EastNeighbour->IsUpdatingMesh)
            {
                EastNeighbour->UpdateRoadMesh();
            }
        }
        if (GridCellRef->WNeighbour)
        {
            ACB_RoadCell* WestNeighbour = Cast<ACB_RoadCell>((*(GridCellRef->WNeighbour))->OccupyingActor);
            if (WestNeighbour && !WestNeighbour->IsUpdatingMesh)
            {
                WestNeighbour->UpdateRoadMesh();
            }
        }
    }
}

void ACB_RoadCell::DestroyRoad() {
    if (GridCellRef) {
        RoadRef->Destroy();
        GridCellRef->SetUnoccupied();
        UpdateNeighbours();
        Destroy();
    }
}
