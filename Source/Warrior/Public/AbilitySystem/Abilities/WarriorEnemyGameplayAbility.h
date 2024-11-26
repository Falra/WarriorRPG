// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorEnemyGameplayAbility.generated.h"

class UEnemyCombatComponent;
class AWarriorEnemyCharacter;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorEnemyGameplayAbility : public UWarriorGameplayAbility
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|Ability")
    AWarriorEnemyCharacter* GetEnemyCharacterFromActorInfo();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|Ability")
    UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|Ability")
    FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat);


private:
    TWeakObjectPtr<AWarriorEnemyCharacter> CachedEnemyCharacter;
};