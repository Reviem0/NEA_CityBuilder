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

    if (GridCellRef && !(GetClass()->IsChildOf(ACB_RoadTile::StaticClass()))) {
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
        if (!(GridCellRef->NNeighbour) || !(GridCellRef->SNeighbour) || !(GridCellRef->ENeighbour) || !(GridCellRef->WNeighbour))
        {
            return;
        }

        // Check which neighbours are road tiles
        bool NorthR = (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road;
        bool SouthR = (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road;
        bool EastR = (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road;
        bool WestR = (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road;

        // Select Correct Actor and Rotation
        // If all neighbours are road tiles, use the cross actor
        
        if (NorthR && SouthR && EastR && WestR)
        {
            // Spawn New Actor
            GetWorld()->SpawnActor<ARoadCross>(RoadCross, GetActorLocation(), GetActorRotation());
            if (!neighbourUpdate)
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            // Destroy Old Actor
            this->Destroy();
            return;
        }
        // If 3 Neighbours are road tiles, use the T actor
        if (EastR && SouthR && WestR)
        {
            FRotator NewRot = FRotator(0, 0, 0);
            GetWorld()->SpawnActor<ARoadT>(RoadT, GetActorLocation(), NewRot);
            
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }

        if (NorthR && SouthR && WestR)
        {
            FRotator NewRot = FRotator(0, 90, 0);
            GetWorld()->SpawnActor<ARoadT>(RoadT, GetActorLocation(), NewRot);
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }

        if (NorthR && SouthR && EastR)
        {
            FRotator NewRot = FRotator(0, 180, 0);
            GetWorld()->SpawnActor<ARoadT>(RoadT, GetActorLocation(), NewRot);
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }

        if (NorthR && WestR && EastR)
        {
            FRotator NewRot = FRotator(0, 270, 0);
            GetWorld()->SpawnActor<ARoadT>(RoadT, GetActorLocation(), NewRot);
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }

        // If 2 Neighbours are road tiles, use the straight actor
        if (NorthR && SouthR)
        {
            FRotator NewRot = FRotator(0, 0, 0);
            GetWorld()->SpawnActor<ARoadStraight>(RoadStraight, GetActorLocation(), NewRot);
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }

        if (WestR && EastR)
        {
            FRotator NewRot = FRotator(0, 90, 0);
            GetWorld()->SpawnActor<ARoadStraight>(RoadStraight, GetActorLocation(), NewRot);
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }

        // If 1 Neighbour is a road tile, use the end actor
        if (NorthR)
        {
            FRotator NewRot = FRotator(0, 0, 0);
            GetWorld()->SpawnActor<ARoadEnd>(RoadEnd, GetActorLocation(), NewRot);
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }

        if (SouthR)
        {
            FRotator NewRot = FRotator(0, 180, 0);
            GetWorld()->SpawnActor<ARoadEnd>(RoadEnd, GetActorLocation(), NewRot);
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }

        if (EastR)
        {
            FRotator NewRot = FRotator(0, 90, 0);
            GetWorld()->SpawnActor<ARoadEnd>(RoadEnd, GetActorLocation(), NewRot);
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }

        if (WestR)
        {
            FRotator NewRot = FRotator(0, 270, 0);
            GetWorld()->SpawnActor<ARoadEnd>(RoadEnd, GetActorLocation(), NewRot);
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }

        // If 0 Neighbours are road tiles, use the circle actor
        if (!NorthR && !SouthR && !EastR && !WestR)
        {
            FRotator NewRot = FRotator(0, 0, 0);
            GetWorld()->SpawnActor<ARoadCircle>(RoadCircle, GetActorLocation(), NewRot);
           
            if (!neighbourUpdate) 
            {
                UpdateNeighbours();
            }
            IsUpdatingMesh = false;
            this->Destroy();
            return;
        }
    }
    IsUpdatingMesh = false;
}

void ACB_RoadTile::UpdateNeighbours()
{
    UE_LOG(LogTemp, Warning, TEXT("UpdateNeighbours called for tile at %s"), *GetActorLocation().ToString());

    // Check which neighbours are road tiles
    AGridCell* NNeighbour = *(GridCellRef->NNeighbour);
    AGridCell* SNeighbour = *(GridCellRef->SNeighbour);
    AGridCell* ENeighbour = *(GridCellRef->ENeighbour);
    AGridCell* WNeighbour = *(GridCellRef->WNeighbour);

    bool NorthR = (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road;
    bool SouthR = (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road;
    bool EastR = (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road;
    bool WestR = (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road;

    if (NorthR && !Cast<ACB_RoadTile>(NNeighbour->OccupyingActor)->IsUpdatingMesh)
    {
        // Get the actor and cast to road tile
        (Cast<ACB_RoadTile>(NNeighbour->OccupyingActor))->UpdateRoadMesh(true);
    }

    if (SouthR && !Cast<ACB_RoadTile>(SNeighbour->OccupyingActor)->IsUpdatingMesh)
    {
        // Get the actor and cast to road tile
        (Cast<ACB_RoadTile>(SNeighbour->OccupyingActor))->UpdateRoadMesh(true);
    }

    if (EastR && !Cast<ACB_RoadTile>(ENeighbour->OccupyingActor)->IsUpdatingMesh)
    {
        // Get the actor and cast to road tile
        (Cast<ACB_RoadTile>(ENeighbour->OccupyingActor))->UpdateRoadMesh(true);
    }

    if (WestR && !Cast<ACB_RoadTile>(WNeighbour->OccupyingActor)->IsUpdatingMesh)
    {
        // Get the actor and cast to road tile
        (Cast<ACB_RoadTile>(WNeighbour->OccupyingActor))->UpdateRoadMesh(true);
    }
    UE_LOG(LogTemp, Warning, TEXT("UpdateNeighbours finished for tile at %s"), *GetActorLocation().ToString());
    return;
}
