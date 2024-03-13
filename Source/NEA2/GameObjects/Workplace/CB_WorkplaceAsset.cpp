// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_WorkplaceAsset.h"
#include "../CB_Workplace.h"

// Sets default values
ACB_WorkplaceAsset::ACB_WorkplaceAsset()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    BuildingType = EBuildingType::Workplace;
    BuildingClass = EBuildingClass::None;

    
}

// Called when the game starts or when spawned
void ACB_WorkplaceAsset::BeginPlay()
{
    Super::BeginPlay();

    if (WorkplaceRef){
        BuildingClass = (Cast<ACB_Workplace>(WorkplaceRef))->BuildingClass;
    }

    // Set Material
    TArray<UStaticMeshComponent*> MeshComponents;

    GetComponents<UStaticMeshComponent>(MeshComponents);

    for (UStaticMeshComponent* MeshComponent : MeshComponents) {


        switch (BuildingClass)
        {
            case EBuildingClass::Red:
                MeshComponent->SetMaterial(0, RedMAT);
                break;
            case EBuildingClass::Blue:
                MeshComponent->SetMaterial(0, BlueMAT);
                break;
            case EBuildingClass::Green:
                   MeshComponent->SetMaterial(0, GreenMAT);
                break;
            case EBuildingClass::Yellow:
                MeshComponent->SetMaterial(0, YellowMAT);
                break;
        }
    }    

}

// Called every frame
void ACB_WorkplaceAsset::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

