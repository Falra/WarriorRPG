// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorProjectileBase.generated.h"

class UProjectileMovementComponent;
class UNiagaraComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
    OnHit,
    OnBeginOverlap
};

UCLASS()
class WARRIOR_API AWarriorProjectileBase : public AActor
{
    GENERATED_BODY()

public:
    AWarriorProjectileBase();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<UBoxComponent> ProjectileCollisionBox;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<UNiagaraComponent> ProjectileNiagaraComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    EProjectileDamagePolicy ProjectileDamagePolicy = EProjectileDamagePolicy::OnHit;

    UFUNCTION()
    virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawn Projectile Hit FX"))
    void BP_OnSpawnProjectileHitFX(const FVector& HitLocation);
};