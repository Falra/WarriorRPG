// Valerii Diachenko All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"

#include "WarriorAbilitySystemComponent.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "Abilities/WarriorHeroGameplayAbility.h"

void UDataAsset_HeroStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASC, int32 ApplyLevel)
{
    Super::GiveToAbilitySystemComponent(InASC, ApplyLevel);

    for (const auto& HeroAbilitySet : HeroStartUpAbilities)
    {
        if (!HeroAbilitySet.IsValid())
        {
            continue;
        }
        FGameplayAbilitySpec AbilitySpec(HeroAbilitySet.Ability, ApplyLevel);
        AbilitySpec.SourceObject = InASC->GetAvatarActor();
        AbilitySpec.DynamicAbilityTags.AddTag(HeroAbilitySet.InputTag);
        
        InASC->GiveAbility(AbilitySpec);
    }
}