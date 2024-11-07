// Valerii Diachenko All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"

#include "GameplayAbilitySpec.h"
#include "WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASC, int32 ApplyLevel)
{
    Super::GiveToAbilitySystemComponent(InASC, ApplyLevel);

    if (EnemyCombatAbilities.IsEmpty())
    {
        return;
    }

    for (const auto& AbilityClass : EnemyCombatAbilities)
    {
        if (!AbilityClass)
        {
            continue;
        }
        
        FGameplayAbilitySpec AbilitySpec(AbilityClass, ApplyLevel);
        AbilitySpec.SourceObject = InASC->GetAvatarActor();
        
        InASC->GiveAbility(AbilitySpec);
    }
}