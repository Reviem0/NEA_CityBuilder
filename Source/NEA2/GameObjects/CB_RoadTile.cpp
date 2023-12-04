// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_RoadTile.h"
#include "../Grid/GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ChildActorComponent.h"

ACB_RoadTile::ACB_RoadTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACB_RoadTile::BeginPlay()
{
	Super::BeginPlay();
    BuildingType = EBuildingType::Road;
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    if (GridManager) {
        GridCellRef = GridManager->GetClosestGridCell(GetActorLocation());
    }

    if (GridCellRef && !isPlop) {
        GridCellRef->SetOccupied(EBuildingType::Road, this);
        isOcc = true;
        if (!isPlop)
        {
            UpdateRoadMesh();
        }
    }

     if (GridCellRef)
    {
        OldNorthR = GridCellRef->NNeighbour && ((*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::House);
        OldSouthR = GridCellRef->SNeighbour && ((*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::House);
        OldEastR = GridCellRef->ENeighbour && ((*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::House);
        OldWestR = GridCellRef->WNeighbour && ((*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::House);
    }

}

// Called every frame
void ACB_RoadTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACB_RoadTile::SpawnNewActor(UClass* ActorClass, FRotator Rotation)
{
    ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(ActorClass, GetActorLocation(), Rotation);
    NewSpawn(NewActor);
    UpdateNeighbours();
    IsUpdatingMesh = false;
    if (NewActor){
        Destroy();
    }
}

void ACB_RoadTile::UpdateRoadMesh()
{
    IsUpdatingMesh = true;

    if (GridCellRef)
    {
        bool NorthR = GridCellRef->NNeighbour && ((*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::House);
        bool SouthR = GridCellRef->SNeighbour && ((*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::House);
        bool EastR = GridCellRef->ENeighbour && ((*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::House);
        bool WestR = GridCellRef->WNeighbour && ((*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::House);

        int roadCount = NorthR + SouthR + EastR + WestR;

        if (NorthR == OldNorthR && SouthR == OldSouthR && EastR == OldEastR && WestR == OldWestR && (NorthR || SouthR || EastR || WestR))
        {
            IsUpdatingMesh = false;
            return;
        }

        switch (roadCount)
        {
            case 4:
            {
                SpawnNewActor(RoadCross, GetActorRotation());
                break;
            }
            case 3:
            {
                FRotator T_Rot = EastR && SouthR && WestR ? FRotator(0, 90, 0) : NorthR && SouthR && WestR ? FRotator(0, 0, 0) : NorthR && SouthR && EastR ? FRotator(0, 180, 0) : FRotator(0, 270, 0);
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
                    FRotator Corner_Rot = NorthR && EastR ? FRotator(0, 0, 0) : SouthR && EastR ? FRotator(0, 270, 0) : SouthR && WestR ? FRotator(0, 180, 0) : FRotator(0, 90, 0);
                    SpawnNewActor(RoadCorner, Corner_Rot);
                }
                break;
            }
            case 1:
            {
                FRotator End_Rot = NorthR ? FRotator(0, 0, 0) : SouthR ? FRotator(0, 180, 0) : EastR ? FRotator(0, 270, 0) : FRotator(0, 90, 0);
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

void ACB_RoadTile::UpdateNeighbours()
{
    if (GridCellRef )
    {
        if (GridCellRef->NNeighbour)
        {
            ACB_RoadTile* NorthNeighbour = Cast<ACB_RoadTile>((*(GridCellRef->NNeighbour))->OccupyingActor);
            if (NorthNeighbour && !NorthNeighbour->IsUpdatingMesh)
            {
                NorthNeighbour->UpdateRoadMesh();
            }
        }
        if (GridCellRef->SNeighbour)
        {
            ACB_RoadTile* SouthNeighbour = Cast<ACB_RoadTile>((*(GridCellRef->SNeighbour))->OccupyingActor);
            if (SouthNeighbour && !SouthNeighbour->IsUpdatingMesh)
            {
                SouthNeighbour->UpdateRoadMesh();
            }
        }
        if (GridCellRef->ENeighbour)
        {
            ACB_RoadTile* EastNeighbour = Cast<ACB_RoadTile>((*(GridCellRef->ENeighbour))->OccupyingActor);
            if (EastNeighbour && !EastNeighbour->IsUpdatingMesh)
            {
                EastNeighbour->UpdateRoadMesh();
            }
        }
        if (GridCellRef->WNeighbour)
        {
            ACB_RoadTile* WestNeighbour = Cast<ACB_RoadTile>((*(GridCellRef->WNeighbour))->OccupyingActor);
            if (WestNeighbour && !WestNeighbour->IsUpdatingMesh)
            {
                WestNeighbour->UpdateRoadMesh();
            }
        }
    }
}

void ACB_RoadTile::NewSpawn(ACB_RoadTile *NewRoadTile)
{
    if (NewRoadTile){
        
        NewRoadTile->GridCellRef = GridCellRef;
        NewRoadTile->isOcc = true;
        NewRoadTile->LastGridRef = GridCellRef;
        GridCellRef->SetOccupied(EBuildingType::Road, NewRoadTile);

        NewRoadTile->RoadCross = RoadCross;
        NewRoadTile->RoadT = RoadT;
        NewRoadTile->RoadStraight = RoadStraight;
        NewRoadTile->RoadCorner = RoadCorner;
        NewRoadTile->RoadEnd = RoadEnd;
        NewRoadTile->RoadCircle = RoadCircle;
        
    }
}

void ACB_RoadTile::DestroyRoad() {
    if (GridCellRef) {
        GridCellRef->SetUnoccupied();
        isOcc = false;
        UpdateNeighbours();
        Destroy();
    }
}

/* void ACB_RoadTile::UpdateRoadMesh(boo)
{
    IsUpdatingMesh = true;

    if (GridCellRef)
    {
        if (!(GridCellRef->NNeighbour) && !(GridCellRef->SNeighbour) && !(GridCellRef->ENeighbour) && !(GridCellRef->WNeighbour))
        {
            return;
        }

        bool NorthR = false;
        bool SouthR = false;
        bool EastR = false;
        bool WestR = false;

        // Check which neighbours are road tiles
        if (GridCellRef->NNeighbour)
        {
            NorthR = (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::House;
        }
        
        if (GridCellRef->SNeighbour)
        {
            SouthR = (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::House;
        }

        if (GridCellRef->ENeighbour)
        {
            EastR = (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::House;
        }

        if (GridCellRef->WNeighbour)
        {
            WestR = (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::House;
        }
        
        // Select Correct Actor and Rotation
        // If all neighbours are road tiles, use the cross actor
        
        if (NorthR && SouthR && EastR && WestR)
        {
            // Spawn New Actor
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadCross, GetActorLocation(), GetActorRotation());
            NewSpawn(NewActor);
            if )
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            // Destroy Old Actor
            if (NewActor){
                Destroy();
            }
            return;
        }
        // If 3 Neighbours are road tiles, use the T actor
        if (EastR && SouthR && WestR)
        {
            FRotator NewRot = FRotator(0, 90, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadT, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
            
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        if (NorthR && SouthR && WestR)
        {
            FRotator NewRot = FRotator(0, 0, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadT, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        if (NorthR && SouthR && EastR)
        {
            FRotator NewRot = FRotator(0, 180, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadT, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        if (NorthR && WestR && EastR)
        {
            FRotator NewRot = FRotator(0, 270, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadT, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        // If 2 Neighbours are road tiles, use the straight actor
        if (NorthR && SouthR)
        {
            FRotator NewRot = FRotator(0, 90, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadStraight, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        if (WestR && EastR)
        {
            FRotator NewRot = FRotator(0, 0, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadStraight, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        // If 2 Neighbours are road tiles, use the corner actor

        if (NorthR && EastR)
        {
            FRotator NewRot = FRotator(0, 0, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadCorner, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        if (SouthR && EastR)
        {
            FRotator NewRot = FRotator(0, 270, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadCorner, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        if (SouthR && WestR)
        {
            FRotator NewRot = FRotator(0, 180, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadCorner, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;

        }

        if (NorthR && WestR)
        {
            FRotator NewRot = FRotator(0, 90, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadCorner, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        // If 1 Neighbour is a road tile, use the end actor
        if (NorthR)
        {
            FRotator NewRot = FRotator(0, 0, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadEnd, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        if (SouthR)
        {
            FRotator NewRot = FRotator(0, 180, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadEnd, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        if (EastR)
        {
            FRotator NewRot = FRotator(0, 270, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadEnd, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        if (WestR)
        {
            FRotator NewRot = FRotator(0, 90, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadEnd, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            return;
        }

        // If 0 Neighbours are road tiles, use the circle actor
        if (!NorthR && !SouthR && !EastR && !WestR)
        {
            FRotator NewRot = FRotator(0, 0, 0);
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadCircle, GetActorLocation(), NewRot);
            NewSpawn(NewActor);
           
            if ) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            if (NewActor){
                Destroy();
            }
            UE_LOG(LogTemp, Warning, TEXT("Destroy called on actor %s"), *GetName());
            return;
        }
    }
    IsUpdatingMesh = false;
} */