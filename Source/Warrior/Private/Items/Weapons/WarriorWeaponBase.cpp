// Valerii Diachenko All Rights Reserved.


#include "Items/Weapons/WarriorWeaponBase.h"

#include "WarriorDebugHelper.h"
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
    WeaponCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnWeaponCollisionBeginOverlap);
    WeaponCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnWeaponCollisionEndOverlap);
}

void AWarriorWeaponBase::OnWeaponCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    const auto* WeaponOwningPawn = GetInstigator<APawn>();

    checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the owning pawn for the weapon: %s"), *GetName())

    if (const auto* HitPawn = Cast<APawn>(OtherActor))
    {
        if (WeaponOwningPawn != HitPawn)
        {
            OnWeaponHitTarget.ExecuteIfBound(OtherActor);
        }

        // TODO: Implement hit check for enemy characters
    }
}

void AWarriorWeaponBase::OnWeaponCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
    const auto* WeaponOwningPawn = GetInstigator<APawn>();

    checkf(WeaponOwningPawn,TEXT("Forgot to assign an instigator as the owning pawn for the weapon: %s"),*GetName());

    if (const auto* HitPawn = Cast<APawn>(OtherActor))
    {
        if (WeaponOwningPawn != HitPawn)
        {
            OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
        }

        // TODO: Implement hit check for enemy characters
    }
}