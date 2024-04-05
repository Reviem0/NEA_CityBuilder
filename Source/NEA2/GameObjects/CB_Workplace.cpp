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
    
    if (!GridCellRef) {
        UE_LOG(LogTemp, Warning, TEXT("WORKPLACE: GridCellRef is null"));
        DestroyWorkplace();
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

    
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    if ((!BottomLeft || !TopRight || !TopLeft || !BottomRight || !RoadPlacement) || (BottomLeft->OccupyingType != EBuildingType::None || TopRight->OccupyingType != EBuildingType::None || TopLeft->OccupyingType != EBuildingType::None || BottomRight->OccupyingType != EBuildingType::None || RoadPlacement->OccupyingType != EBuildingType::None || GridManager->PlayGridArray.Contains(RoadPlacement) == false))
    {
        UE_LOG(LogTemp, Warning, TEXT("WORKPLACE: NOT ENOUGH CELLS"));
        DestroyWorkplace();
        return;
    }
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
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

    if (RoadPlacement->OccupyingActor->StaticClass() == ACB_OwnedRoadCell::StaticClass()) {
        Cast<ACB_OwnedRoadCell>(RoadPlacement->OccupyingActor)->OwningCells.Add(GridCellRef);
    } else {
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
    GetWorld()->GetTimerManager().SetTimer(GoalTimerHandle, this, &ACB_Workplace::GoalNotMet, GoalTimer, false);
}

// Called every frame
void ACB_Workplace::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACB_Workplace::CarArrived(AActor * Car)
{
    AddScore();
    ACB_CarAI* CarAI = Cast<ACB_CarAI>(Car);
    // Add HouseOrigin to the queue
    if (Cast<AActor>(CarAI->OriginHouse)) {
        AddToHouseQueue(Cast<AActor>(CarAI->OriginHouse));
    }
    UE_LOG(LogTemp, Display, TEXT("Car arrived at workplace"));
    HoldingCurrent++;
    if (HoldingCurrent >= HoldingCapacity) {
        IsFull = true;
    }
    


}
void ACB_Workplace::AddToHouseQueue(AActor *House)
{
    HouseQueue.Add(House);
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACB_Workplace::OnWaitFinished, cooldown, false);

}

void ACB_Workplace::OnWaitFinished() {
    UE_LOG(LogTemp, Display, TEXT("RAN"));
    // Remove the first item from the queue
    if (HouseQueue.Num() > 0) {
        bool Success = CreatePath(HouseQueue[0]);

        // If Path was unsuccessful, add the house to the back of the queue
        if (!Success){
            AddToHouseQueue(HouseQueue[0]);
        } else {
            HoldingCurrent--;
        }
        HouseQueue.RemoveAt(0);
        if (HoldingCurrent < HoldingCapacity) {
            IsFull = false;
        } else {
            IsFull = true;
        }
    }

    // Do something when the wait is finished
}

bool ACB_Workplace::CreatePath(AActor* House) {
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
    if (!GridManager) {
        UE_LOG(LogTemp, Error, TEXT("GridManager is null"));
        return false;
    }

    if (!House) {
        UE_LOG(LogTemp, Error, TEXT("House is null"));
        return false;
    }

    ACB_House* HouseCell = Cast<ACB_House>(House);
    if (!HouseCell) {
        UE_LOG(LogTemp, Error, TEXT("HouseCell is null"));
        return false;
    }

    if (!RoadTileAsset || !HouseCell->RoadTileAsset) {
        UE_LOG(LogTemp, Error, TEXT("RoadTileAsset is null"));
        return false;
    }

    TArray<AGridCell*> Path = GridManager->FindPath(RoadTileAsset->GridCellRef, HouseCell->RoadTileAsset->GridCellRef);
    if (Path.Num() > 0) {
        CreateSpline(Path, House);
    } else {
        if (Spline) {
            Spline->RemoveFromRoot();
            Spline->DestroyComponent();
            Spline = nullptr;
        }
        return false;
    }

    return true;
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
            } else {
                PreviousRightVector = FVector(0,0,0);
            }
        }

        int multiplier = 10;
        if (turning) {
            multiplier = 20;
        }

        // log the right vector with spline point for debugging purposes
        // UE_LOG(LogTemp, Display, TEXT("Spline point %d: %s"), i, *RightVector.ToString());36

        // draw debug line for right vector
        // DrawDebugLine(GetWorld(), Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World), Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) - (RightVector + PreviousRightVector) * multiplier, FColor::Blue, true, 10.0f, -1, 1.0f);
        
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

void ACB_Workplace::AddScore() {
    Points++;
    AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
    if (GameManager) {
        UE_LOG(LogTemp, Display, TEXT("WORKPLACE: Score: %d"), Points);
        GameManager->AddScore(1);
        CurrentScore++;
        if (CurrentScore >= Goal) {
            IncreaseGoal();
            // Reset goal timer
            if (isCritical) {
                isCritical = false;
                GetWorld()->GetTimerManager().ClearTimer(CriticalTimerHandle);
            }
            GetWorld()->GetTimerManager().SetTimer(GoalTimerHandle, this, &ACB_Workplace::GoalNotMet, 240, false);
        }
    } else {
        UE_LOG(LogTemp, Warning, TEXT("WORKPLACE: GameManager is null"));
    }
}

void ACB_Workplace::DestroyWorkplace() {
    GridCellRef->SetUnoccupied();
    Destroy();
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
    AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
    Goal += 20;
    GameManager->SatisfactionCheck();
}

void ACB_Workplace::LossCondition()
{
    AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
    if (isCritical) {
        isLost = true;
    }
    if (GameManager) {
        GameManager->LossFunction();
    }
}

float ACB_Workplace::GetCriticalValue()
{
    if (CriticalTimerHandle.IsValid()) {
        float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(CriticalTimerHandle);
        return TimeRemaining/CriticalTime;
    }
    return 0;
}
