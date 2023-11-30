// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_House.h"
#include "../../Grid/GridManager.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ACB_House::ACB_House()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::House;
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
    if (North) AvailableRotations.Add(FRotator(0, 0, 0));
    if (East) AvailableRotations.Add(FRotator(0, 90, 0));
    if (South) AvailableRotations.Add(FRotator(0, 180, 0));
    if (West) AvailableRotations.Add(FRotator(0, 270, 0));

    // Generate a random index within the range of the list's size
    int32 RandomIndex = FMath::RandRange(0, AvailableRotations.Num() - 1);

    // Use the random index to select a rotation from the list
    SetActorRotation(AvailableRotations[RandomIndex]);

    BuildingClass = EBuildingClass::Red;
    
}

// Called every frame
void ACB_House::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (GridCellRef) {
        
    }
    
}



