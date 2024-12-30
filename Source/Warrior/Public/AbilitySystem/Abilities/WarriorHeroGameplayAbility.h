// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"

class UHeroUIComponent;
class UHeroCombatComponent;
class AWarriorHeroController;
class AWarriorHeroCharacter;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorHeroGameplayAbility : public UWarriorGameplayAbility
{
    GENERATED_BODY()

public:
    
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|Ability")
    AWarriorHeroCharacter* GetHeroCharacterFromActorInfo();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|Ability")
    AWarriorHeroController* GetHeroControllerFromActorInfo();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|Ability")
    UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|Ability")
    FGameplayEffectSpecHandle MakeHeroDamageSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float WeaponBaseDamage, FGameplayTag AttackTypeTag, int32 ComboCount);

    UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
    UHeroUIComponent* GetHeroUIComponentFromActorInfo();
    
    UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
    bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime);
    
private:

    TWeakObjectPtr<AWarriorHeroCharacter> CachedHeroCharacter;

    TWeakObjectPtr<AWarriorHeroController> CachedHeroController;
};