// Valerii Diachenko All Rights Reserved.


#include "WarriorStructTypes.h"

#include "Abilities/WarriorHeroGameplayAbility.h"

bool FWarriorHeroAbilitySet::IsValid() const
{
    return InputTag.IsValid() && Ability;
}