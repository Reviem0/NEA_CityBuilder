// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_Camera.h"
#include "../Grid/GridCell.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ACB_Camera::ACB_Camera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Spawn Arrow Component
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeRotation(FRotator(270, 0, 0));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void ACB_Camera::BeginPlay()
{
	Super::BeginPlay();

	// Get player controller and set view target to this camera
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(this, 0.0);
	
}

// Called every frame
void ACB_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If GridManager is not set, get GridManager
	if (!GridManager) {
		GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridManager::StaticClass()));
		if (GridManager){
			UE_LOG(LogTemp, Warning, TEXT("GridManager Found"));
		} else {
			UE_LOG(LogTemp, Warning, TEXT("GridManager Not Found"));
		}
	}
	else
	{
		// Get Furthest Grid Cell
		TArray<AGridCell*> GridArray = GridManager->PlayGridArray;
		AActor* FurthestCell = GridArray[0];
		for (AActor* Cell : GridArray)
		{
			// If the distance between the current cell and the camera is greater than the distance between the furthest cell and the camera
			if (FMath::Abs((Cell->GetActorLocation() - GetActorLocation()).Size()) > FMath::Abs((FurthestCell->GetActorLocation() - GetActorLocation()).Size()))
			{
				FurthestCell = Cell;
			}
		}

		// Convert GridArray to TArray<AActor*>
		TArray<AActor*> ActorGridArray;
		for (AGridCell* GridCell : GridArray)
		{
			ActorGridArray.Add(GridCell);
		}

		// Set Camera Location. This aims to center the camera to the grid
		FVector Location = UGameplayStatics::GetActorArrayAverageLocation(ActorGridArray); // Get the average location of all the grid cells
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), Location, GetWorld()->DeltaTimeSeconds, 5)); // Interpolate the camera location to the average location of all the grid cells

		// Set Camera Distance. This aims to set the camera distance so that the furthest grid cell is visible
		FVector Vec = (GetActorTransform().InverseTransformPosition(FurthestCell->GetActorLocation())).GetAbs();
		CameraDistance = FVector::Dist(Vec, Vec*-1);

		FVector Current = Camera->GetComponentLocation();
		FVector Target = GetActorForwardVector()*-1*CameraDistance + GetActorLocation();
		FVector CameraLocation = FMath::VInterpTo(Current, Target, GetWorld()->DeltaTimeSeconds, 5);

		// Set Camera Location
		Camera->SetWorldLocation(CameraLocation);

		
	}
}

