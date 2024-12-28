// Valerii Diachenko All Rights Reserved.


#include "WarriorTypes/WarriorStructTypes.h"

#include "Abilities/WarriorHeroGameplayAbility.h"

bool FWarriorHeroAbilitySet::IsValid() const
{
    return InputTag.IsValid() && Ability;
}