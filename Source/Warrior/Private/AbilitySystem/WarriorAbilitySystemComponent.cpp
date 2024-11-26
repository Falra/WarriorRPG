// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Abilities/WarriorHeroGameplayAbility.h"
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

void UWarriorAbilitySystemComponent::RemoveHeroWeaponAbilities(TArray<FGameplayAbilitySpecHandle>& InAbilityHandles)
{
    if (InAbilityHandles.IsEmpty())
    {
        return;
    }
    
    for (const FGameplayAbilitySpecHandle& SpecHandle : InAbilityHandles)
    {
        if (SpecHandle.IsValid())
        {
            ClearAbility(SpecHandle);
        }
    }
    
    InAbilityHandles.Empty();
}

bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
    check(AbilityTagToActivate.IsValid());

    TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

    if (!FoundAbilitySpecs.IsEmpty())
    {
        const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
        FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

        check(SpecToActivate);

        if (!SpecToActivate->IsActive())
        {
            return TryActivateAbility(SpecToActivate->Handle);
        }
    }
    
    return false;
}