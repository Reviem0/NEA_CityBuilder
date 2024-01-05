// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_House.h"
#include "../../Grid/GridManager.h"
#include "DrawDebugHelpers.h"

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
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    if (GridManager){
        GridCellRef = GridManager->GetClosestGridCell(GetActorLocation());
        if (GridCellRef){
            GridCellRef->SetOccupied(BuildingType, this);
        }
    }

    bool North = GridCellRef->NNeighbour != nullptr; // 1
    bool South = GridCellRef->SNeighbour != nullptr; // 2
    bool East = GridCellRef->ENeighbour != nullptr;  // 3
    bool West = GridCellRef->WNeighbour != nullptr;  // 4

    // Create a list to store the available rotations
    TArray<FRotator> AvailableRotations;

    // Check each direction. If a neighbour exists in that direction, add the corresponding rotation to the list
    if (North) AvailableRotations.Add(FRotator(0, 270, 0));
    if (East) AvailableRotations.Add(FRotator(0, 0, 0));
    if (South) AvailableRotations.Add(FRotator(0, 90, 0));
    if (West) AvailableRotations.Add(FRotator(0, 180, 0));

    // Generate a random index within the range of the list's size
    int32 RandomIndex = FMath::RandRange(0, AvailableRotations.Num() - 1);

    // Use the random index to select a rotation from the list
    SetActorRotation(AvailableRotations[RandomIndex]);
    AGridCell* RoadPlacement = nullptr;

    switch (static_cast<int>(GetActorRotation().Yaw))
    {
        case 0:
        {
            RoadPlacement = GridCellRef->ENeighbour && (*(GridCellRef->ENeighbour))->OccupyingType == EBuildingType::None ? *(GridCellRef->ENeighbour) : nullptr;
            break;
        }
        case 90:
        {
            RoadPlacement = GridCellRef->SNeighbour && (*(GridCellRef->SNeighbour))->OccupyingType == EBuildingType::None ? *(GridCellRef->SNeighbour) : nullptr;
            break;
        }
        case 180:
        {
            RoadPlacement = GridCellRef->WNeighbour && (*(GridCellRef->WNeighbour))->OccupyingType == EBuildingType::None ? *(GridCellRef->WNeighbour) : nullptr;
            break;
        }
        case 270:
        {
            RoadPlacement = GridCellRef->NNeighbour && (*(GridCellRef->NNeighbour))->OccupyingType == EBuildingType::None ? *(GridCellRef->NNeighbour) : nullptr;
            break;
        }
    }

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (RoadTileActor)
    {   
        RoadTileAsset = GetWorld()->SpawnActor<ACB_OwnedRoadCell>(RoadTileActor, RoadPlacement->GetActorLocation(), FRotator(0,0,0), SpawnInfo);
        if (RoadTileAsset) {
            RoadTileAsset->OwningCell = GridCellRef;
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
                if (!Spline){
                    CreateSpline(Path);
                }
            } else {
                if (Spline) {
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
    if (Path.Num() == 0) return;
    Spline = NewObject<USplineComponent>(this, USplineComponent::StaticClass());
    Spline->RegisterComponent();
    Spline->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
    Spline->SetWorldLocation(RoadTileAsset->GetActorLocation()); // Set the spline's location to the house's location
    Spline->SetLocationAtSplinePoint(1, RoadTileAsset->GetActorLocation(), ESplineCoordinateSpace::World); // Set the first point of the spline to the house's location
    // Remove first point from spline
    Spline->RemoveSplinePoint(0);

    // Make spline follow path
    for (int i = 0; i < Path.Num(); i++)
    {
        Spline->AddSplinePoint(Path[i]->GetActorLocation(), ESplineCoordinateSpace::World);
    }

    // Log the number of spline points
    UE_LOG(LogTemp, Display, TEXT("Number of spline points: %d"), Spline->GetNumberOfSplinePoints());

    UE_LOG(LogTemp, Display, TEXT("Spline created"));
}

void ACB_House::PathCheck()
{
    if(Spline) {
        ACB_CarAI* Car = GetWorld()->SpawnActor<ACB_CarAI>(CarAI, RoadTileAsset->GetActorLocation(), FRotator(0,0,0));
        if (Car){
            Car->FollowSpline(Spline);
        }
    }
}