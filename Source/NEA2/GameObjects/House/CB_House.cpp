// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_House.h"
#include "Kismet/GameplayStatics.h"
#include "../../Grid/GridManager.h"
#include "DrawDebugHelpers.h"
#include "../CB_OwnedRoadCell.h"

#include "Kismet/GameplayStatics.h"



// Sets default values
ACB_House::ACB_House()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::House;
    BuildingClass = EBuildingClass::Red;
}

// Called when the game starts or when spawned
void ACB_House::BeginPlay()
{
    Super::BeginPlay();

    // Check if available rotations are valid
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    bool North = (GridCellRef->NNeighbour != nullptr) && ((*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road) && GridManager->PlayGridArray.Contains(*(GridCellRef->NNeighbour)); // 1
    bool South = (GridCellRef->SNeighbour != nullptr) && ((*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road) && GridManager->PlayGridArray.Contains(*(GridCellRef->SNeighbour)); // 2
    bool East  = (GridCellRef->ENeighbour != nullptr) && ((*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road) && GridManager->PlayGridArray.Contains(*(GridCellRef->ENeighbour)); // 3
    bool West  = (GridCellRef->WNeighbour != nullptr) && ((*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road) && GridManager->PlayGridArray.Contains(*(GridCellRef->WNeighbour)); // 4


  // Destroy if no avaliable rotations
    if (!North && !South && !East && !West) {
        GridCellRef->SetUnoccupied();
        Destroy();
        UE_LOG(LogTemp, Warning, TEXT("No available rotations for house placement. Destroying actor."));
        return;
    }

    // Create a list to store the available rotations
    TArray<int> AvailableRotations;

  
    // Check each direction. If a neighbour exists in that direction, add the corresponding rotation to the list
    if (North) AvailableRotations.Add(270);
    if (East) AvailableRotations.Add(0);
    if (South) AvailableRotations.Add(90);
    if (West) AvailableRotations.Add(180);

    // print all available rotations for debugging purposes 
    for (int i = 0; i < AvailableRotations.Num(); i++)
    {
        UE_LOG(LogTemp, Display, TEXT("Available Rotation: %d"), AvailableRotations[i]);
    }
    AGridCell* RoadPlacement = nullptr;

    // Loop until a valid road placement is found
    while (RoadPlacement == nullptr && AvailableRotations.Num() > 0)
    {
        // Generate a random index within the range of the list's size
        int RandomIndex = FMath::RandRange(0, AvailableRotations.Num() - 1);

        // Use the random index to select a rotation from the list
        int yaw = AvailableRotations[RandomIndex];
        SetActorRotation(FRotator(0, yaw, 0));

        switch (static_cast<int>(GetActorRotation().Yaw))
        {
            case 0:
            {
                if (GridCellRef->ENeighbour && (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road)
                    RoadPlacement = *(GridCellRef->ENeighbour);
                    if (GridCellRef->ENeighbour && (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road) {
                        Cast<ACB_RoadCell>((*(GridCellRef->ENeighbour))->OccupyingActor)->DestroyRoad();
                    }
                else
                    AvailableRotations.RemoveAt(RandomIndex);
                break;
            }
            case 90:
            {
                if (GridCellRef->SNeighbour && (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road)
                    RoadPlacement = *(GridCellRef->SNeighbour);
                    if (GridCellRef->SNeighbour && (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road) {
                        Cast<ACB_RoadCell>((*(GridCellRef->SNeighbour))->OccupyingActor)->DestroyRoad();
                    }
                else
                    AvailableRotations.RemoveAt(RandomIndex);
                break;
            }
            case 180:
            {
                if (GridCellRef->WNeighbour && (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road)
                    RoadPlacement = *(GridCellRef->WNeighbour);
                    if (GridCellRef->WNeighbour && (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road) {
                        Cast<ACB_RoadCell>((*(GridCellRef->WNeighbour))->OccupyingActor)->DestroyRoad();
                    }
                else
                    AvailableRotations.RemoveAt(RandomIndex);
                break;
            }
            case 270:
            {
                if (GridCellRef->NNeighbour && (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road)
                    RoadPlacement = *(GridCellRef->NNeighbour);
                    if (GridCellRef->NNeighbour && (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road) {
                        Cast<ACB_RoadCell>((*(GridCellRef->NNeighbour))->OccupyingActor)->DestroyRoad();
                    }
                else
                    AvailableRotations.RemoveAt(RandomIndex);
                break;
            }
        }
    }
    

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (RoadPlacement->OccupyingActor->StaticClass() == ACB_OwnedRoadCell::StaticClass()) {
        Cast<ACB_OwnedRoadCell>(RoadPlacement->OccupyingActor)->OwningCells.Add(GridCellRef);
    } else {
        if (RoadTileActor && RoadPlacement)
        {   
            RoadTileAsset = GetWorld()->SpawnActorDeferred<ACB_OwnedRoadCell>(RoadTileActor, FTransform(RoadPlacement->GetActorLocation()));
            if (RoadTileAsset) {
                // Add the current cell to the road's list of owning cells
                RoadTileAsset->OwningCells.Add(GridCellRef);
                UGameplayStatics::FinishSpawningActor(RoadTileAsset, FTransform(RoadPlacement->GetActorLocation()));
            }
        }
    }

    // Set Material
    TArray<UStaticMeshComponent*> MeshComponents;

    GetComponents<UStaticMeshComponent>(MeshComponents);

    for (UStaticMeshComponent* MeshComponent : MeshComponents) {

        
        switch (BuildingClass)
        {
            case EBuildingClass::Red:
                MeshComponent->SetMaterial(1, RedMAT);
                break;
            case EBuildingClass::Blue:
                MeshComponent->SetMaterial(1, BlueMAT);
                break;
            case EBuildingClass::Green:
                   MeshComponent->SetMaterial(1, GreenMAT);
                break;
            case EBuildingClass::Yellow:
                break;
        }
    }
    
    FTimerHandle MemberTimerHandle;
    UE_LOG(LogTemp, Display, TEXT("House Begin Play"));
    GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ACB_House::PathCheck, 10.0f, true, 2.0f);

}

// Called every frame
void ACB_House::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

void ACB_House::AddTargetWorkplace(ACB_Workplace* Workplace)
{
    TargetWorkplaces.Add(Workplace);
    // sort the workplaces by distance
    TargetWorkplaces.Sort([this](const ACB_Workplace& A, const ACB_Workplace& B) {
        FVector HouseLocation = RoadTileAsset->GridCellRef->GetActorLocation();
        return (FVector::Dist(HouseLocation,A.RoadTileAsset->GridCellRef->GetActorLocation()) < FVector::Dist(HouseLocation,B.RoadTileAsset->GridCellRef->GetActorLocation()));
    });
    CreatePath();
}

void ACB_House::CreatePath()
{
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    if (GridManager && TargetWorkplaces.Num() != 0){
        for (ACB_Workplace* TargetWorkplace : TargetWorkplaces) {
            if (TargetWorkplace == nullptr) return;

            AGridCell* TargetCell = TargetWorkplace->RoadTileAsset->GridCellRef;
            if (TargetCell){
                TArray<AGridCell*> Path = GridManager->FindPath(RoadTileAsset->GridCellRef, TargetCell);
                if (Path.Num() != 0){
                    CreateSpline(Path);
                    break;
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

    if (TargetWorkplaces.Num() == 0) {
        UE_LOG(LogTemp, Display, TEXT("NO WORKPLACES FOUND"));
    }
}


void ACB_House::CreateSpline(TArray<AGridCell*> Path) 
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
    Spline->SetLocationAtSplinePoint(1, GetActorLocation(), ESplineCoordinateSpace::World); // Set the first point of the spline to the house's location
    // Remove first point from spline
    Spline->RemoveSplinePoint(0);

    // Add the origin road tile to the start of the path
    Path.Insert(RoadTileAsset->GridCellRef, 0); 
    // Add The target workplace to the end of the path
    Path.Add(TargetWorkplaces[0]->BottomLeftAsset->GridCellRef);

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
}

void ACB_House::PathCheck()
{
    if(Spline && CarAvailability > 0) {
        ACB_CarAI* Car = GetWorld()->SpawnActor<ACB_CarAI>(CarAI, RoadTileAsset->GetActorLocation(), FRotator(0,0,0));
        UE_LOG(LogTemp, Display, TEXT("Car spawned"));
        if (Car){
            Car->OriginHouse = this;
            Car->DestinationWorkplace = TargetWorkplaces[0];
            Car->FollowSpline(Spline);
            CarAvailability--;
        }
    }
}

void ACB_House::CarArrived(ACB_CarAI *Car)
{
    CarAvailability++;
}