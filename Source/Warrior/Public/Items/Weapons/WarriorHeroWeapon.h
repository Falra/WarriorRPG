// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "WarriorStructTypes.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
    GENERATED_BODY()

public:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
    FWarriorHeroWeaponData HeroWeaponData;

    UFUNCTION(BlueprintCallable)
    void AssignGrantedAbilityHandles(const TArray<FGameplayAbilitySpecHandle>& InAbilityHandles);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    TArray<FGameplayAbilitySpecHandle> GetGrantedAbilityHandles() const;
private:
    
    TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

    
};