// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"

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

private:

    TWeakObjectPtr<AWarriorHeroCharacter> CachedHeroCharacter;

    TWeakObjectPtr<AWarriorHeroController> CachedHeroController;
};