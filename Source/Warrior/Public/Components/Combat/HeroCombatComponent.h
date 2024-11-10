// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class AWarriorHeroWeapon;
/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroCombatComponent : public UPawnCombatComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|Combat")
    AWarriorHeroWeapon* GetHeroCarriedWeaponByTag(FGameplayTag GameplayTag) const;

    virtual void OnHitTargetActor(AActor* HitActor) override;
    virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;
};