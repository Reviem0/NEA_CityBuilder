// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_OwnedRoadCell.h"

ACB_OwnedRoadCell::ACB_OwnedRoadCell()
{
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::Road;
    Ownership = EOwnership::Game;
    
}

// Sets default values
void ACB_OwnedRoadCell::BeginPlay(){
    Super::BeginPlay();
    

}

// Called every frame
void ACB_OwnedRoadCell::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACB_OwnedRoadCell::UpdateRoadMesh()
{
    IsUpdatingMesh = true;

    if (GridCellRef)
    {
        bool NorthR = GridCellRef->NNeighbour && ((*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road || IsCellOwner(*(GridCellRef->NNeighbour)));
        bool SouthR = GridCellRef->SNeighbour && ((*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road || IsCellOwner(*(GridCellRef->SNeighbour)));
        bool EastR = GridCellRef->ENeighbour && ((*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road || IsCellOwner(*(GridCellRef->ENeighbour)));
        bool WestR = GridCellRef->WNeighbour && ((*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road || IsCellOwner(*(GridCellRef->WNeighbour)));

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

bool ACB_OwnedRoadCell::IsCellOwner(AGridCell *Cell)
{
    if (Cell)
    {
         
        for (auto& OwningCell : OwningCells)
        {
            if (OwningCell == Cell)
            {
                return true;
            }
        }
    }
    return false;
}