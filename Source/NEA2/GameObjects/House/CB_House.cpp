// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_House.h"
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
    bool North = GridCellRef->NNeighbour != nullptr && (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::None; // 1
    bool South = GridCellRef->SNeighbour != nullptr && (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::None; // 2
    bool East  = GridCellRef->ENeighbour != nullptr && (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::None; // 3
    bool West  = GridCellRef->WNeighbour != nullptr && (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::None; // 4

  // Destroy if no avaliable rotations
    if (!North && !South && !East && !West) {
        Destroy();
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
                if (GridCellRef->ENeighbour && (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::None)
                    RoadPlacement = *(GridCellRef->ENeighbour);
                else
                    AvailableRotations.RemoveAt(RandomIndex);
                break;
            }
            case 90:
            {
                if (GridCellRef->SNeighbour && (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::None)
                    RoadPlacement = *(GridCellRef->SNeighbour);
                else
                    AvailableRotations.RemoveAt(RandomIndex);
                break;
            }
            case 180:
            {
                if (GridCellRef->WNeighbour && (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::None)
                    RoadPlacement = *(GridCellRef->WNeighbour);
                else
                    AvailableRotations.RemoveAt(RandomIndex);
                break;
            }
            case 270:
            {
                if (GridCellRef->NNeighbour && (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::None)
                    RoadPlacement = *(GridCellRef->NNeighbour);
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
            RoadTileAsset = GetWorld()->SpawnActor<ACB_OwnedRoadCell>(RoadTileActor, RoadPlacement->GetActorLocation(), FRotator(0,0,0), SpawnInfo);
            if (RoadTileAsset) {
                RoadTileAsset->OwningCells.Add(GridCellRef);
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
        }
    }
    
    FTimerHandle MemberTimerHandle;

    GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ACB_House::PathCheck, 10.0f, true, 2.0f);

}

// Called every frame
void ACB_House::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

void ACB_House::CreatePath()
{
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    if (GridManager && TargetWorkplaces.Num() != 0){
        ACB_Workplace* TargetWorkplace = TargetWorkplaces[0];
        if (TargetWorkplace == nullptr) return;

        AGridCell* TargetCell = TargetWorkplace->RoadTileAsset->GridCellRef;
        if (TargetCell){
            TArray<AGridCell*> Path = GridManager->FindPath(RoadTileAsset->GridCellRef, TargetCell);
            if (Path.Num() != 0){
                CreateSpline(Path);
            } else {
                if (Spline) {
                    Spline->RemoveFromRoot();
                    Spline->DestroyComponent();
                    Spline = nullptr;
                }
            }
        }
    } else {
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