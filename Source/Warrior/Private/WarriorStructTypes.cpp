// Valerii Diachenko All Rights Reserved.


#include "WarriorStructTypes.h"

#include "Abilities/WarriorGameplayAbility.h"

bool FWarriorHeroAbilitySet::IsValid() const
{
    return InputTag.IsValid() && Ability;
}