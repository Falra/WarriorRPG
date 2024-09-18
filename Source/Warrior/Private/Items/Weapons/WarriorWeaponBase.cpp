// Valerii Diachenko All Rights Reserved.


#include "Items/Weapons/WarriorWeaponBase.h"

#include "Components/BoxComponent.h"

AWarriorWeaponBase::AWarriorWeaponBase()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);

    WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
    WeaponCollision->SetupAttachment(GetRootComponent());
    WeaponCollision->SetBoxExtent(FVector(20.0f));
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
