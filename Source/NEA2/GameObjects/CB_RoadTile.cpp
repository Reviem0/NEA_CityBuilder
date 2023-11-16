// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_RoadTile.h"
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

    if (GridCellRef && !isPlop) {
        GridCellRef->SetOccupied(EBuildingType::Road, this);
        isOcc = true;
        if (!isPlop)
        {
            UpdateRoadMesh(false);
        }
    }
}

// Called every frame
void ACB_RoadTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (!isOcc) {
        BeginPlay();
    }
}

void ACB_RoadTile::UpdateRoadMesh(bool neighbourUpdate)
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
            NorthR = (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road;
        }
        if (GridCellRef->SNeighbour)
        {
            SouthR = (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road;
        }
        if (GridCellRef->ENeighbour)
        {
            EastR = (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road;
        }
        if (GridCellRef->WNeighbour)
        {
            WestR = (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road;
        }

        // Select Correct Actor and Rotation
        // If all neighbours are road tiles, use the cross actor
        
        if (NorthR && SouthR && EastR && WestR)
        {
            // Spawn New Actor
            ACB_RoadTile* NewActor = GetWorld()->SpawnActor<ACB_RoadTile>(RoadCross, GetActorLocation(), GetActorRotation());
            NewSpawn(NewActor);
            if (!neighbourUpdate)
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
            
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
           
            if (!neighbourUpdate) 
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
}

void ACB_RoadTile::UpdateNeighbours()
{
    UE_LOG(LogTemp, Warning, TEXT("UpdateNeighbours called for tile at %s"), *GetActorLocation().ToString());

    bool NorthR = false;
    bool SouthR = false;
    bool EastR = false;
    bool WestR = false;

    if (GridCellRef->NNeighbour)
    {
        NorthR = (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road;
        AGridCell* NNeighbour = *(GridCellRef->NNeighbour);

        if (NorthR && !Cast<ACB_RoadTile>(NNeighbour->OccupyingActor)->IsUpdatingMesh)
        {
            // Get the actor and cast to road tile
            (Cast<ACB_RoadTile>(NNeighbour->OccupyingActor))->UpdateRoadMesh(true);
        }
    }
    if (GridCellRef->SNeighbour)
    {
        SouthR = (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road;
        AGridCell* SNeighbour = *(GridCellRef->SNeighbour);
        if (SouthR && !Cast<ACB_RoadTile>(SNeighbour->OccupyingActor)->IsUpdatingMesh)
        {
            // Get the actor and cast to road tile
            (Cast<ACB_RoadTile>(SNeighbour->OccupyingActor))->UpdateRoadMesh(true);
        }
    }
    if (GridCellRef->ENeighbour)
    {
        EastR = (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road;
        AGridCell* ENeighbour = *(GridCellRef->ENeighbour);
        if (EastR && !Cast<ACB_RoadTile>(ENeighbour->OccupyingActor)->IsUpdatingMesh)
        {
            // Get the actor and cast to road tile
            (Cast<ACB_RoadTile>(ENeighbour->OccupyingActor))->UpdateRoadMesh(true);
        }
    }
    if (GridCellRef->WNeighbour)
    {
        WestR = (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road;
        AGridCell* WNeighbour = *(GridCellRef->WNeighbour);
        if (WestR && !Cast<ACB_RoadTile>(WNeighbour->OccupyingActor)->IsUpdatingMesh)
        {
            // Get the actor and cast to road tile
            (Cast<ACB_RoadTile>(WNeighbour->OccupyingActor))->UpdateRoadMesh(true);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("UpdateNeighbours finished for tile at %s"), *GetActorLocation().ToString());
    return;
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