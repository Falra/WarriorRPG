// Valerii Diachenko All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "WarriorAbilitySystemComponent.h"
#include "Abilities/WarriorGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASC, int32 ApplyLevel)
{
    check(InASC);

    GrantAbilities(InASC, ActivateOnGivenAbilities, ApplyLevel);
    GrantAbilities(InASC, ReactiveAbilities, ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(UWarriorAbilitySystemComponent* InASC,
    const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilities, int32 ApplyLevel)
{
    if (InAbilities.IsEmpty())
    {
        return;
    }

    for (const TSubclassOf<UWarriorGameplayAbility>& Ability : InAbilities)
    {
        if (!IsValid(Ability))
        {
            continue;
        }

        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, ApplyLevel);
        AbilitySpec.SourceObject = InASC->GetAvatarActor();
        
        InASC->GiveAbility(AbilitySpec);
    }
}