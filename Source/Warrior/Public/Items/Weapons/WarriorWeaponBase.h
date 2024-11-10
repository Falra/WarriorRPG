// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorWeaponBase.generated.h"

class UBoxComponent;

UCLASS()
class WARRIOR_API AWarriorWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    AWarriorWeaponBase();

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
    TObjectPtr<UStaticMeshComponent> WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
    TObjectPtr<UBoxComponent> WeaponCollision;

    UFUNCTION()
    void OnWeaponCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnWeaponCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

public:
    FORCEINLINE UBoxComponent* GetWeaponCollision() const { return WeaponCollision; }
};