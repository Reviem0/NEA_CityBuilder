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
    bool North = (GridCellRef->NNeighbour != nullptr) && ((*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::OwnedRoad) && GridManager->PlayGridArray.Contains(*(GridCellRef->NNeighbour)); // 1
    bool South = (GridCellRef->SNeighbour != nullptr) && ((*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::OwnedRoad) && GridManager->PlayGridArray.Contains(*(GridCellRef->SNeighbour)); // 2
    bool East  = (GridCellRef->ENeighbour != nullptr) && ((*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::OwnedRoad) && GridManager->PlayGridArray.Contains(*(GridCellRef->ENeighbour)); // 3
    bool West  = (GridCellRef->WNeighbour != nullptr) && ((*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::OwnedRoad) && GridManager->PlayGridArray.Contains(*(GridCellRef->WNeighbour)); // 4


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

    // Log the available rotations for debugging purposes
    for (int i = 0; i < AvailableRotations.Num(); i++) {
        UE_LOG(LogTemp, Display, TEXT("Available Rotation: %d"), AvailableRotations[i]);
    }

    if (AvailableRotations.Num() == 0) {
        GridCellRef->SetUnoccupied();
        Destroy();
        UE_LOG(LogTemp, Warning, TEXT("No available rotations for house placement. Destroying actor."));
        return;
    }
    AGridCell* RoadPlacement = nullptr;

    // Loop until a valid road placement is found
    while (RoadPlacement == nullptr && AvailableRotations.Num() > 0)
    {
        // Generate a random index within the range of the list's size
        int RandomIndex = FMath::RandRange(0, AvailableRotations.Num() - 1);

        // Use the random index to select a rotation from the list

        int yaw = AvailableRotations[RandomIndex];
        // Log the selected rotation for debugging purposes
        UE_LOG(LogTemp, Display, TEXT("Selected Rotation: %d"), yaw);
        SetActorRotation(FRotator(0, yaw, 0));

        switch (yaw)
        {
            case 0:
            {
                if (GridCellRef->ENeighbour && (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::OwnedRoad)
                    RoadPlacement = *(GridCellRef->ENeighbour);
                    UE_LOG(LogTemp, Warning, TEXT("Attempting to place road at east neighbour."))
                    if (GridCellRef->ENeighbour && (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::Road) {
                        Cast<ACB_RoadCell>((*(GridCellRef->ENeighbour))->OccupyingActor)->DestroyRoad();
                    }
                else
                    AvailableRotations.RemoveAt(RandomIndex);
                break;
            }
            case 90:
            {
                RoadPlacement = *(GridCellRef->SNeighbour);
                UE_LOG(LogTemp, Warning, TEXT("Attempting to place road at south neighbour."))
                if (RoadPlacement == nullptr) {
                    UE_LOG(LogTemp, Warning, TEXT("RoadPlacement is null. Removing rotation south"));
                    AvailableRotations.RemoveAt(RandomIndex);
                    break;
                }
                if (GridCellRef->SNeighbour && (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::Road) {
                    Cast<ACB_RoadCell>((*(GridCellRef->SNeighbour))->OccupyingActor)->DestroyRoad();
                }
                break;
            }
            case 180:
            {
                if (GridCellRef->WNeighbour && (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::None || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road || (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::OwnedRoad)
                    RoadPlacement = *(GridCellRef->WNeighbour);
                    UE_LOG(LogTemp, Warning, TEXT("Attempting to place road at west neighbour."))
                    if (GridCellRef->WNeighbour && (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::Road) {
                        Cast<ACB_RoadCell>((*(GridCellRef->WNeighbour))->OccupyingActor)->DestroyRoad();
                    }
                else
                    AvailableRotations.RemoveAt(RandomIndex);
                break;
            }
            case 270:
            {
                RoadPlacement = *(GridCellRef->NNeighbour);
                UE_LOG(LogTemp, Warning, TEXT("Attempting to place road at north neighbour."))
                if (RoadPlacement == nullptr) {
                    UE_LOG(LogTemp, Warning, TEXT("RoadPlacement is null. Removing rotation north"));
                    AvailableRotations.RemoveAt(RandomIndex);
                    break;
                }
                if (GridCellRef->NNeighbour && (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::Road) {
                    Cast<ACB_RoadCell>((*(GridCellRef->NNeighbour))->OccupyingActor)->DestroyRoad();
                }
                break;
            }
        }
    }

    if (AvailableRotations.Num() == 0) {
        GridCellRef->SetUnoccupied();
        Destroy();
        UE_LOG(LogTemp, Warning, TEXT("No available rotations for house placement. Destroying actor."));
        return;
    }
    

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (RoadPlacement->OccupyingActor->StaticClass() == ACB_OwnedRoadCell::StaticClass()) {
        Cast<ACB_OwnedRoadCell>(RoadPlacement->OccupyingActor)->OwningCells.Add(GridCellRef);
    } else {
        if (RoadTileActor && RoadPlacement)
        {
            if (RoadPlacement->OccupyingType == EBuildingType::OwnedRoad){
                Cast<ACB_OwnedRoadCell>(RoadPlacement->OccupyingActor)->OwningCells.Add(GridCellRef);
                Cast<ACB_OwnedRoadCell>(RoadPlacement->OccupyingActor)->UpdateRoadMesh();
            }
            if (RoadPlacement->OccupyingType == EBuildingType::None){
                RoadTileAsset = GetWorld()->SpawnActorDeferred<ACB_OwnedRoadCell>(RoadTileActor, FTransform(RoadPlacement->GetActorLocation()));
                if (RoadTileAsset) {
                    // Add the current cell to the road's list of owning cells
                    RoadTileAsset->OwningCells.Add(GridCellRef);
                    UGameplayStatics::FinishSpawningActor(RoadTileAsset, FTransform(RoadPlacement->GetActorLocation()));
                }
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
    // Set a timer to check for a path every 10 seconds
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
    CreatePath();
}

void ACB_House::AddTargetWorkplace(TArray<ACB_Workplace*> Workplaces)
{
    TargetWorkplaces.Append(Workplaces);
    CreatePath();
}

void ACB_House::SortWorkplaces()
{
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    // clear the dictionary
    WorkplaceDistances.Empty();

    // sort the workplaces by the distance between the house and the workplace not path distance
    TargetWorkplaces.Sort([this](const ACB_Workplace& A, const ACB_Workplace& B) {
        return FVector::Dist(GetActorLocation(), A.GetActorLocation()) < FVector::Dist(GetActorLocation(), B.GetActorLocation());
    });

    // Create a dictionary to store the distance between the house and the workplace
    int count = 0;
    for (ACB_Workplace* TargetWorkplace : TargetWorkplaces) {
        // Break the loop after the fifth workplace
        if (count >= 5) {
            break;
        }

        // Check if the workplace is valid
        if (TargetWorkplace == nullptr) {
            UE_LOG(LogTemp, Error, TEXT("TargetWorkplace is null"));
            continue;
        }
        // Check if the workplace's road tile asset is valid
        if (TargetWorkplace->RoadTileAsset == nullptr) {
            UE_LOG(LogTemp, Error, TEXT("RoadTileAsset is null"));
            continue;
        }

        // Get the cell of the workplace's road tile asset
        AGridCell* TargetCell = TargetWorkplace->RoadTileAsset->GridCellRef;

        // Find the path between the house and the workplace
        if (TargetCell && TargetWorkplace->IsFull == false){
            TArray<AGridCell*> Path = GridManager->FindPath(RoadTileAsset->GridCellRef, TargetCell);
            // If the path is not empty, add the path to the dictionary
            if (Path.Num() != 0){
                WorkplaceDistances.Add(TargetWorkplace, Path);
            }
        }

        count++;
    }

    // sort the dictionary by the distance between the house and the workplace
    WorkplaceDistances.ValueSort([this](const TArray<AGridCell*>& A, const TArray<AGridCell*>& B) {
        // get workplace by key
        const ACB_Workplace* AWorkplace = *WorkplaceDistances.FindKey(A);
        const ACB_Workplace* BWorkplace = *WorkplaceDistances.FindKey(B);
        int AcriticalPoints = AWorkplace->isCritical ? 10 : 0; // if the workplace is critical, subtract 10 cells from the path
        int BcriticalPoints = BWorkplace->isCritical ? 10 : 0; // if the workplace is critical, subtract 10 cells from the path
        int pointDisparity = AWorkplace->CurrentScore - BWorkplace->CurrentScore; // if the workplace has a higher score, add the difference to the path
        return A.Num() + pointDisparity - AcriticalPoints < B.Num() - BcriticalPoints;
    });
}

void ACB_House::CreatePath()
{
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    if (!GridManager) {
        UE_LOG(LogTemp, Error, TEXT("GridManager is null"));
        return;
    }

    if (Spline) {
        Spline->RemoveFromRoot();
        Spline->DestroyComponent();
        Spline = nullptr;
    }
    SortWorkplaces();
    if (WorkplaceDistances.Num() != 0){
        // Create a spline to first workplace in the dictionary
        CreateSpline(WorkplaceDistances.begin().Value(), WorkplaceDistances.begin().Key());
    } else {
        UE_LOG(LogTemp, Display, TEXT("NO WORKPLACES FOUND"));
    }
}


void ACB_House::CreateSpline(TArray<AGridCell*> Path, ACB_Workplace* TargetWorkplace) 
{
    // If the spline exists, remove it
    if (Spline) {
        Spline->RemoveFromRoot();
        Spline->DestroyComponent();
        Spline = nullptr;
    }
    
    // If the path is empty, return
    if (Path.Num() == 0) return;

    // Create and setup new spline component
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
    Path.Add(TargetWorkplace->BottomLeftAsset->GridCellRef);

    // Make spline follow path
    for (int i = 0; i < Path.Num(); i++)
    {
        // Add the spline point to the spline
        Spline->AddSplinePoint(Path[i]->GetActorLocation(), ESplineCoordinateSpace::World);

        // Set the spline point type to linear to check for turns
        Spline->SetSplinePointType(i, ESplinePointType::Linear, true);

        // Get the right vector at the spline point
        FVector RightVector = Spline->GetRightVectorAtSplinePoint(i, ESplineCoordinateSpace::World);
        // Get the right vector at the previous spline point
        FVector PreviousRightVector = FVector(0,0,0);
        bool turning = false;
        // check if the right vector of the previous point is the same as the current point
        if (i > 0) {
            PreviousRightVector = Spline->GetRightVectorAtSplinePoint(i-1, ESplineCoordinateSpace::World);
            // If the right vector is not the same, the car is turning
            if (!PreviousRightVector.Equals(RightVector, 1)) { // .Equals() Helps compensate for floating point errors by checking if the vectors are within 1 unit of each other
                turning = true;
                UE_LOG(LogTemp, Display, TEXT("Turning"));
            } else {
                // If the car is not turning, set the previous right vector to 0
                PreviousRightVector = FVector(0,0,0);
            }
        }

        // Set the multiplier for how far to shift the spline point
        int multiplier = 10;
        if (turning) {
            // If the car is turning, set the multiplier to 20
            multiplier = 20;
        }

        // log the right vector with spline point for debugging purposes
        UE_LOG(LogTemp, Display, TEXT("Spline point %d: %s"), i, *RightVector.ToString());

        // draw debug line to show the shift
        DrawDebugLine(GetWorld(), Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World), Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) - (RightVector + PreviousRightVector) * multiplier, FColor::Red, true, 10.0f, -1, 1.0f);
        
        // Shift the spline point to the left by subtracting the right vector from the spline point
        Spline->SetLocationAtSplinePoint(i, Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) - (RightVector + PreviousRightVector) * multiplier, ESplineCoordinateSpace::World);
        // Reset the spline point type to curve
        Spline->SetSplinePointType(i, ESplinePointType::Curve, true);
    }

    // Log the number of spline points
    UE_LOG(LogTemp, Display, TEXT("Number of spline points: %d"), Spline->GetNumberOfSplinePoints());

    UE_LOG(LogTemp, Display, TEXT("Spline created"));
}

void ACB_House::PathCheck()
{
    // If the spline is valid, Send a car
    if(Spline && CarAvailability > 0) {
        // Spawn a car
        ACB_CarAI* Car = GetWorld()->SpawnActor<ACB_CarAI>(CarAI, RoadTileAsset->GetActorLocation(), FRotator(0,0,0));
        // Log that the car has been spawned for debugging purposes
        UE_LOG(LogTemp, Display, TEXT("Car spawned"));
        // If the car spawns successfully, set the origin house and destination workplace, and make the car follow the spline
        if (Car){
            Car->OriginHouse = this;
            // Set the destination workplace to the first workplace in the dictionary
            Car->DestinationWorkplace = WorkplaceDistances.begin().Key();
            Car->FollowSpline(Spline);
            // Decrement the car availability
            CarAvailability--;
        }
    }
}

void ACB_House::CarArrived(ACB_CarAI *Car)
{
    // Increment the car availability
    CarAvailability++;
}