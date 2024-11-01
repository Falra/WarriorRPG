// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"

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