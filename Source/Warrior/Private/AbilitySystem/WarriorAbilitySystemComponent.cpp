// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Abilities/WarriorGameplayAbility.h"
#include "WarriorStructTypes.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
    {
        return;
    }

    for (const FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            TryActivateAbility(AbilitySpec.Handle);
            break;
        }
    }
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
    
}

void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutAbilityHandles)
{
    if (InAbilities.IsEmpty())
    {
        return;
    }

    for (const FWarriorHeroAbilitySet& AbilitySet : InAbilities)
    {
        if (!AbilitySet.IsValid())
        {
            continue;
        }

        FGameplayAbilitySpec AbilitySpec(AbilitySet.Ability, ApplyLevel);
        AbilitySpec.SourceObject = GetAvatarActor();
        AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

        OutAbilityHandles.AddUnique(GiveAbility(AbilitySpec));
    }
}