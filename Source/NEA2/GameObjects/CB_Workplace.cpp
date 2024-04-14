// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_Workplace.h"

#include "Kismet/GameplayStatics.h"
#include "CarAI/CB_CarAI.h"
#include "../Grid/GridManager.h"
#include "../GameManager.h"
#include "CarAI/CB_CarAI.h"
#include "../Grid/GridManager.h"
#include "../HUDElements/CB_CriticalBar.h"

ACB_Workplace::ACB_Workplace() 
{
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::None;
    BuildingClass = EBuildingClass::None;

    CriticalBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("CriticalBar"));
    CriticalBarComp->SetupAttachment(RootComponent);


    Goal = 20;
}

// Sets default values
void ACB_Workplace::BeginPlay()
{
    Super::BeginPlay();
    
    // If the workplace is not valid, destroy it
    if (!GridCellRef) {
        UE_LOG(LogTemp, Warning, TEXT("WORKPLACE: GridCellRef is null"));
        DestroyWorkplace();
        return;
    }

    //UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Orientation: %d"), Orientation);

    // Get grid manager
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    AGridCell* BottomLeft = GridCellRef;
    AGridCell* BottomRight = GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
    AGridCell* TopLeft = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
    AGridCell* TopRight = BottomRight && BottomRight->NNeighbour ? *(BottomRight->NNeighbour) : nullptr;
    
    AGridCell* RoadPlacement = BottomLeft->SNeighbour && GridManager->PlayGridArray.Contains(*(BottomLeft->SNeighbour)) ? *(BottomLeft->SNeighbour) : nullptr;
    FRotator* Rotation = new FRotator(0,0,0);

       // Choose a random orientation for the workplace
    int Orientation = FMath::RandRange(0, 3);

    switch (Orientation)
    {
        case 0: // South Facing
        {
            UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Orientation: 0"));
            BottomLeft = GridCellRef;
            BottomRight = GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
            TopLeft = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
            TopRight = BottomRight && BottomRight->NNeighbour ? *(BottomRight->NNeighbour) : nullptr;
    
            RoadPlacement = BottomLeft->SNeighbour && GridManager->PlayGridArray.Contains(*(BottomLeft->SNeighbour))  ? *(BottomLeft->SNeighbour) : nullptr;
            Rotation = new FRotator(0,0,0);
            break;
        }
        case 1: // West Facing
        {
            UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Orientation: 1"));
            BottomLeft = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
            BottomRight = GridCellRef;
            TopRight = GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
            TopLeft = BottomLeft && BottomLeft->ENeighbour ? *(BottomLeft->ENeighbour) : nullptr;

            RoadPlacement = BottomLeft->WNeighbour && GridManager->PlayGridArray.Contains(*(BottomLeft->WNeighbour)) ? *(BottomLeft->WNeighbour) : nullptr;
            Rotation = new FRotator(0,90,0);
            break;
        }
        case 2: // East Facing
        {
            UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Orientation: 2"));
            BottomLeft = GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
            BottomRight = BottomLeft && BottomLeft->NNeighbour ? *(BottomLeft->NNeighbour) : nullptr;
            TopRight = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
            TopLeft = GridCellRef;

            RoadPlacement = BottomLeft->ENeighbour && GridManager->PlayGridArray.Contains(*(BottomLeft->ENeighbour)) ? *(BottomLeft->ENeighbour) : nullptr;
            Rotation = new FRotator(0,270,0);
            break;
        }
        case 3: // North Facing
        {
            UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Orientation: 3"));
            BottomRight = GridCellRef->NNeighbour ? *(GridCellRef->NNeighbour) : nullptr;
            TopRight = GridCellRef;
            TopLeft = GridCellRef && GridCellRef->ENeighbour ? *(GridCellRef->ENeighbour) : nullptr;
            BottomLeft = BottomRight && BottomRight->ENeighbour ? *(BottomRight->ENeighbour) : nullptr;

            RoadPlacement = BottomLeft && BottomLeft->NNeighbour && GridManager->PlayGridArray.Contains(*(BottomLeft->NNeighbour)) ? *(BottomLeft->NNeighbour) : nullptr;
            Rotation = new FRotator(0,180,0);
            break;
        }
    }


    // Create an FRotator from Rotation
    FRotator Rot(0, Rotation->Yaw, 0);


    // Check if all the cells are valid and unoccupied and if not, destroy the workplace
    if (
        (!BottomLeft || !TopRight || !TopLeft || !BottomRight || !RoadPlacement) || // Check if the cells are valid
        (BottomLeft->OccupyingType != EBuildingType::None || TopRight->OccupyingType != EBuildingType::None || // Check if the cells are unoccupied
        TopLeft->OccupyingType != EBuildingType::None || 
        BottomRight->OccupyingType != EBuildingType::None || 
        RoadPlacement->OccupyingType != EBuildingType::None || 
        GridManager->GridArray.Contains(RoadPlacement) == false) // Check if the road placement is valid
        )
    {
        UE_LOG(LogTemp, Warning, TEXT("WORKPLACE: NOT ENOUGH CELLS"));
        DestroyWorkplace();
        return;
    }
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    // Spawn the BottomLeftAsset
    if (BottomLeftActor)
    {
        BottomLeftAsset = GetWorld()->SpawnActorDeferred<ACB_WorkplaceAsset>(BottomLeftActor, FTransform(BottomLeft->GetActorLocation()));
        if (BottomLeftAsset) {
            BottomLeft->SetActorRotation(Rot);
            BottomLeft->Manager = this;
            BottomLeftAsset->WorkplaceRef = this;
            UGameplayStatics::FinishSpawningActor(BottomLeftAsset, BottomLeft->GetActorTransform());
        }
    }

    // Spawn the TopRightAsset
    if (TopRightActor)
    {
        TopRightAsset = GetWorld()->SpawnActorDeferred<ACB_WorkplaceAsset>(TopRightActor,FTransform(BottomLeft->GetActorLocation()));
        if (TopRightAsset) {
            TopRight->SetActorRotation(Rot);   
            TopRight->Manager = this;
            TopRightAsset->WorkplaceRef = this;
            UGameplayStatics::FinishSpawningActor(TopRightAsset, TopRight->GetActorTransform());
        }
    }

    // Spawn the TopLeftAsset
    if (TopLeftActor)
    {
        TopLeftAsset = GetWorld()->SpawnActorDeferred<ACB_WorkplaceAsset>(TopLeftActor, FTransform(BottomLeft->GetActorLocation()));
        if (TopLeftAsset) {
            TopLeft->SetActorRotation(Rot);
            TopLeft->Manager = this;
            TopLeftAsset->WorkplaceRef = this;
            UGameplayStatics::FinishSpawningActor(TopLeftAsset, TopLeft->GetActorTransform());
        }
    }

    // Spawn the BottomRightAsset
    if (BottomRightActor)
    {
        BottomRightAsset = GetWorld()->SpawnActorDeferred<ACB_WorkplaceAsset>(BottomRightActor, FTransform(BottomLeft->GetActorLocation()));
        if (BottomRightAsset) {
            BottomRight->SetActorRotation(Rot);
            BottomRight->Manager = this;
            BottomRightAsset->WorkplaceRef = this;
            UGameplayStatics::FinishSpawningActor(BottomRightAsset, BottomRight->GetActorTransform());
        }
    }

    // Check if the RoadPlacement is occupied by an owned road cell, if so, add the workplace to the owning cells
    if (RoadPlacement->OccupyingActor->StaticClass() == ACB_OwnedRoadCell::StaticClass()) {
        Cast<ACB_OwnedRoadCell>(RoadPlacement->OccupyingActor)->OwningCells.Add(GridCellRef);
    } else { // Otherwise, spawn a new road cell and add the workplace to the owning cells
            if (RoadTileActor)
                {   
                    RoadPlacement->Manager = this;
                    RoadTileAsset = GetWorld()->SpawnActorDeferred<ACB_OwnedRoadCell>(RoadTileActor, FTransform(Rot,RoadPlacement->GetActorLocation()));
                    if (RoadTileAsset) {
                        RoadTileAsset->OwningCells.Add(BottomLeft);
                        UGameplayStatics::FinishSpawningActor(RoadTileAsset, RoadPlacement->GetActorTransform());
                    }
                }
    }

    // Set the critical bar's owner workplace
    UCB_CriticalBar* CriticalBar = Cast<UCB_CriticalBar>(CriticalBarComp->GetUserWidgetObject());
    if (CriticalBar) {
        CriticalBar->SetOwnerWorkplace(this);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("WORKPLACE: CriticalBar is null"));
    }
    // Set how long the workplace has to reach the goal
    GetWorld()->GetTimerManager().SetTimer(GoalTimerHandle, this, &ACB_Workplace::GoalNotMet, GoalTimer, false);
}

// Called every frame
void ACB_Workplace::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACB_Workplace::DestroyWorkplace() {
    // Set the grid cell to unoccupied
    GridCellRef->SetUnoccupied();
    // Destroy the workplace
    Destroy();
}

void ACB_Workplace::CarArrived(ACB_CarAI* CarAI)
{
    // Add HouseOrigin to the queue
    EnqueueCar(CarAI->OriginHouse);
    UE_LOG(LogTemp, Display, TEXT("Car arrived at workplace"));
    // Add score
    AddScore();
}
void ACB_Workplace::EnqueueCar(ACB_House* House)
{
    // Increment the amount of cars that have arrived
    HoldingCurrent++;
    if (HoldingCurrent >= HoldingCapacity) {
        // Set the workplace to full
        IsFull = true;
    }
    // Add to the back of the queue
    HouseQueue.Add(House);

    // Set a timer for the wait time
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACB_Workplace::DequeueCar, cooldown, false);
}

void ACB_Workplace::DequeueCar() {
    UE_LOG(LogTemp, Display, TEXT("RAN"));
    // Remove the first item from the queue
    if (HouseQueue.Num() > 0) {
        // Create a path to the house
        bool Success = CreatePath(HouseQueue[0]);

        if (!Success){ // If Path was unsuccessful, add the house to the back of the queue
            EnqueueCar(HouseQueue[0]);
        } else { // Otherwise, decrement the amount of cars in the workplace
            HoldingCurrent--;
        }

        // Pop the first item from the queue
        HouseQueue.RemoveAt(0);
        // Recheck if the workplace is full
        if (HoldingCurrent < HoldingCapacity) {
            IsFull = false;
        } else {
            IsFull = true;
        }
    }
}

bool ACB_Workplace::CreatePath(ACB_House* House) {
    // Get the GridManager
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));

    // Check if the GridManager and House are valid
    if (!GridManager) {
        UE_LOG(LogTemp, Error, TEXT("GridManager is null"));
        return false;
    }
    if (!House) {
        UE_LOG(LogTemp, Error, TEXT("House is null"));
        return false;
    }

    if (!RoadTileAsset || !House->RoadTileAsset) {
        UE_LOG(LogTemp, Error, TEXT("RoadTileAsset is null"));
        return false;
    }

    // Find the path from the workplace to the house
    TArray<AGridCell*> Path = GridManager->FindPath(RoadTileAsset->GridCellRef, House->RoadTileAsset->GridCellRef);
    if (Path.Num() > 0) {
        // Create the spline
        CreateSpline(Path, House);
    } else {
        // If the path is empty, delete spline and return false
        if (Spline) {
            Spline->RemoveFromRoot();
            Spline->DestroyComponent();
            Spline = nullptr;
        }
        return false;
    }

    return true;
}

void ACB_Workplace::CreateSpline(TArray<AGridCell *> Path, ACB_House* House)
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
    Spline->SetLocationAtSplinePoint(1, BottomLeftAsset->GetActorLocation(), ESplineCoordinateSpace::World); // Set the first point of the spline to the house's location
    
    // Remove first point from spline
    Spline->RemoveSplinePoint(0);

    // Add the origin road tile to the start of the path
    Path.Insert(RoadTileAsset->GridCellRef, 0);

    // Add The target workplace to the end of the path
    Path.Add(Cast<ACB_House>(House)->GridCellRef);

    // Make spline follow path
    for (int i = 0; i < Path.Num(); i++)
    {
        // Add the spline point to the spline
        Spline->AddSplinePoint(Path[i]->GetActorLocation(), ESplineCoordinateSpace::World);
        
        // Set the spline point type to linear to check for turns
        Spline->SetSplinePointType(i, ESplinePointType::Linear, true);

        // Get the right vector at the spline point
        FVector RightVector = Spline->GetRightVectorAtSplinePoint(i, ESplineCoordinateSpace::World); // Get the right vector at the point
        // Get the right vector at the previous spline point
        FVector PreviousRightVector = FVector(0,0,0);
        bool turning = false;
        // check if the vector of the previous point is the same as the current point
        if (i > 0) {
            PreviousRightVector = Spline->GetRightVectorAtSplinePoint(i-1, ESplineCoordinateSpace::World);
            // If the right vector is not the same, the car is turning
            if (!PreviousRightVector.Equals(RightVector, 1)) { // .Equals() Helps compensate for floating point errors by checking if the vectors are within 1 unit of each other
                turning = true;
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
        // UE_LOG(LogTemp, Display, TEXT("Spline point %d: %s"), i, *RightVector.ToString());36

        // draw debug line for right vector
        // DrawDebugLine(GetWorld(), Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World), Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) - (RightVector + PreviousRightVector) * multiplier, FColor::Blue, true, 10.0f, -1, 1.0f);
        
        // Shift the spline point to the left by subtracting the right vector from the spline point
        Spline->SetLocationAtSplinePoint(i, Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) - (RightVector + PreviousRightVector) * multiplier, ESplineCoordinateSpace::World);
        // Reset the spline point type to curve
        Spline->SetSplinePointType(i, ESplinePointType::Curve, true);
    }

    // Log the number of spline points
    UE_LOG(LogTemp, Display, TEXT("Number of spline points: %d"), Spline->GetNumberOfSplinePoints());

    UE_LOG(LogTemp, Display, TEXT("Spline created"));
    
    // Send the car to the house
    SendCar(House);

}

void ACB_Workplace::SendCar(ACB_House* House) {
    if (Spline) {
        // Spawn the car at the start of the spline
        ACB_CarAI* Car = GetWorld()->SpawnActor<ACB_CarAI>(CarToSpawn, Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World), FRotator(0,0,0));
        if (Car){
            Car->OriginHouse = Cast<ACB_House>(House);
            Car->DestinationWorkplace = this;
            Car->Returning = true;
            Car->FollowSpline(Spline);
        }
    }
}

void ACB_Workplace::AddScore() {
    // Increase the score by 1
    Points++;

    // Get the GameManager
    AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));

    // If the GameManager is valid, add 1 to the score
    if (GameManager) {
        UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Score: %d"), Points);
        // Increase the score by 1
        GameManager->AddScore(1);
        // Increase the current score by 1
        CurrentScore++;

        // If the current score is greater than or equal to the goal, increase the goal
        if (CurrentScore >= Goal) {
            IncreaseGoal();
            // Reset goal timer
            if (isCritical) {
                isCritical = false;
                GetWorld()->GetTimerManager().ClearTimer(CriticalTimerHandle);
            }

            // Set a timer for the goal
            GetWorld()->GetTimerManager().SetTimer(GoalTimerHandle, this, &ACB_Workplace::GoalNotMet, 240, false);
        }
    } else {
        UE_LOG(LogTemp, Warning, TEXT("WORKPLACE: GameManager is null"));
    }
}

void ACB_Workplace::GoalNotMet()
{
    isCritical = true;
    // print critical message with the workplace's name
    UE_LOG(LogTemp, Display, TEXT("WORKPLACE: %s is critical"), *GetName());
    // Set a timer for the loss condition
    GetWorld()->GetTimerManager().SetTimer(CriticalTimerHandle, this, &ACB_Workplace::LossCondition, CriticalTime, false);


}

void ACB_Workplace::IncreaseGoal()
{
    // Get the GameManager
    AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
    // Increase the goal by 20
    CurrentScore = 0;
    Goal += 20;
    UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Goal increased to %d"), Goal);
    // Satisfaction check
    GameManager->SatisfactionCheck();
}

void ACB_Workplace::LossCondition()
{
    // Get the GameManager
    AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
    // Set the workplace to lost
    if (isCritical) {
        isLost = true;
    }
    // Call the loss function in the GameManager
    if (GameManager) {
        GameManager->GameOver();
    }
}

float ACB_Workplace::GetCriticalValue()
{
    // Get the remaining time on the critical timer
    if (CriticalTimerHandle.IsValid()) {
        float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(CriticalTimerHandle);
        return TimeRemaining/CriticalTime;
    }
    return 0;
}
