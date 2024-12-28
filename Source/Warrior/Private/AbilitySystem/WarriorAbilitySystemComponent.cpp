// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"

#include "WarriorGameplayTags.h"
#include "Abilities/WarriorHeroGameplayAbility.h"
#include "WarriorTypes/WarriorStructTypes.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
    {
        return;
    }

    for (const FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
    {
        if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            continue;
        }
        
        if (InputTag.MatchesTag(WarriorGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
        {
            CancelAbilityHandle(AbilitySpec.Handle);
        }
        else
        {
            TryActivateAbility(AbilitySpec.Handle);
            break;
        }
        break;
    }
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid() || !InputTag.MatchesTag(WarriorGameplayTags::InputTag_MustBeHeld))
    {
        return;
    }
    
    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {  
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
        {
            CancelAbilityHandle(AbilitySpec.Handle);
        }
    }
}

void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InDefaultAbilities, const TArray<FWarriorHeroSpecialAbilitySet>& InSpecialAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutAbilityHandles)
{
    if (InDefaultAbilities.IsEmpty() && InSpecialAbilities.IsEmpty())
    {
        return;
    }

    for (const FWarriorHeroAbilitySet& AbilitySet : InDefaultAbilities)
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

    for (const FWarriorHeroSpecialAbilitySet& AbilitySet : InSpecialAbilities)
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