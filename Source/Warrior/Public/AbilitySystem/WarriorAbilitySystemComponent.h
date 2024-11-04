// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "WarriorStructTypes.h"
#include "WarriorAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:

    void OnAbilityInputPressed(const FGameplayTag& InputTag);
    
    void OnAbilityInputReleased(const FGameplayTag& InputTag);

    UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (ApplyLevel = "1"))
    void GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InAbilities, int32 ApplyLevel);
};