// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_Workplace.h"

#include "Kismet/GameplayStatics.h"
#include "CarAI/CB_CarAI.h"
#include "../Grid/GridManager.h"

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

    // Random rotation
    Orientation = FMath::RandRange(0, 3);

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

    if (RoadPlacement->OccupyingActor->StaticClass() == ACB_OwnedRoadCell::StaticClass()) {
        Cast<ACB_OwnedRoadCell>(RoadPlacement->OccupyingActor)->OwningCells.Add(GridCellRef);
    } else {
        if (RoadTileActor)
            {   
                RoadPlacement->Manager = this;
                RoadTileAsset = GetWorld()->SpawnActor<ACB_OwnedRoadCell>(RoadTileActor, RoadPlacement->GetActorLocation(), Rot, SpawnInfo);
                if (RoadTileAsset) {
                    RoadTileAsset->OwningCells.Add(BottomLeft);
                }
            }
    }
}

// Called every frame
void ACB_Workplace::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACB_Workplace::CarArrived(AActor * Car)
{
    Points++;
    ACB_CarAI* CarAI = Cast<ACB_CarAI>(Car);
    // Add HouseOrigin to the queue
    if (Cast<AActor>(CarAI->OriginHouse)) {
        HouseQueue.Add(Cast<AActor>(CarAI->OriginHouse));
    }
    UE_LOG(LogTemp, Display, TEXT("Car arrived at workplace"));
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACB_Workplace::OnWaitFinished, cooldown, false);
    


}
void ACB_Workplace::OnWaitFinished() {
    UE_LOG(LogTemp, Display, TEXT("RAN"));
    // Remove the first item from the queue
    if (HouseQueue.Num() > 0) {
        CreatePath(HouseQueue[0]);
        HouseQueue.RemoveAt(0);
    }

    // Do something when the wait is finished
}

void ACB_Workplace::CreatePath(AActor* House) {
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    if (GridManager && House){
        ACB_House* HouseCell = Cast<ACB_House>(House);
        if (HouseCell) {
            TArray<AGridCell*> Path = GridManager->FindPath(RoadTileAsset->GridCellRef, HouseCell->RoadTileAsset->GridCellRef);
            if (Path.Num() > 0) {
                CreateSpline(Path, House);
            } else {
                if (Spline) {
                    Spline->RemoveFromRoot();
                    Spline->DestroyComponent();
                    Spline = nullptr;
                }
            }
        }
    }
}

void ACB_Workplace::CreateSpline(TArray<AGridCell *> Path, AActor* House)
{
    if (Spline) {
        Spline->RemoveFromRoot();
        Spline->DestroyComponent();
        Spline = nullptr;
    }
    
    if (Path.Num() == 0) return;
    Spline = NewObject<USplineComponent>(this, USplineComponent::StaticClass());
    Spline->RegisterComponent();
    Spline->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
    Spline->SetWorldLocation(RoadTileAsset->GetActorLocation()); // Set the spline's location to the house's location
    Spline->SetLocationAtSplinePoint(1, BottomLeftAsset->GetActorLocation(), ESplineCoordinateSpace::World); // Set the first point of the spline to the house's location
    // Remove first point from spline
    Spline->RemoveSplinePoint(0);

    Path.Insert(RoadTileAsset->GridCellRef, 0);
    Path.Add(Cast<ACB_House>(House)->GridCellRef);
    // Make spline follow path
    for (int i = 0; i < Path.Num(); i++)
    {
        Spline->AddSplinePoint(Path[i]->GetActorLocation(), ESplineCoordinateSpace::World);
        Spline->SetSplinePointType(i, ESplinePointType::Linear, true);

        FVector RightVector = Spline->GetRightVectorAtSplinePoint(i, ESplineCoordinateSpace::World); // Get the right vector at the point
        FVector PreviousRightVector = FVector(0,0,0);
        bool turning = false;
        // check if the vector of the previous point is the same as the current point
        if (i > 0) {
            PreviousRightVector = Spline->GetRightVectorAtSplinePoint(i-1, ESplineCoordinateSpace::World);
            if (!PreviousRightVector.Equals(RightVector, 1)) { // Helps compensate for floating point errors
                turning = true;
                UE_LOG(LogTemp, Display, TEXT("Turning"));
            } else {
                PreviousRightVector = FVector(0,0,0);
            }
        }

        int multiplier = 10;
        if (turning) {
            multiplier = 20;
        }

        // log the right vector with spline point for debugging purposes
        UE_LOG(LogTemp, Display, TEXT("Spline point %d: %s"), i, *RightVector.ToString());

        // draw debug line for right vector
        DrawDebugLine(GetWorld(), Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World), Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) - (RightVector + PreviousRightVector) * multiplier, FColor::Red, true, 10.0f, -1, 1.0f);
        
        Spline->SetLocationAtSplinePoint(i, Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) - (RightVector + PreviousRightVector) * multiplier, ESplineCoordinateSpace::World);
        Spline->SetSplinePointType(i, ESplinePointType::Curve, true);
    }

    // Log the number of spline points
    UE_LOG(LogTemp, Display, TEXT("Number of spline points: %d"), Spline->GetNumberOfSplinePoints());

    UE_LOG(LogTemp, Display, TEXT("Spline created"));

    SendCar(House);



}

void ACB_Workplace::SendCar(AActor* House) {
    if (Spline) {
        ACB_CarAI* Car = GetWorld()->SpawnActor<ACB_CarAI>(CarToSpawn, Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World), FRotator(0,0,0));
        if (Car){
            Car->OriginHouse = Cast<ACB_House>(House);
            Car->DestinationWorkplace = this;
            Car->Returning = true;
            Car->FollowSpline(Spline);
        }
    }
}