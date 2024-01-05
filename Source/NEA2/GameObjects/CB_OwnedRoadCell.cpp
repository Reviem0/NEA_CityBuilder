// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_OwnedRoadCell.h"

ACB_OwnedRoadCell::ACB_OwnedRoadCell()
{
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::Road;
    Ownership = EOwnership::Workplace;
    
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
        bool NorthR = GridCellRef->NNeighbour && ((*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Workplace || (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::House);
        bool SouthR = GridCellRef->SNeighbour && ((*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Workplace || (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::House);
        bool EastR = GridCellRef->ENeighbour && ((*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Workplace || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::House);
        bool WestR = GridCellRef->WNeighbour && ((*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Workplace || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::House);

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