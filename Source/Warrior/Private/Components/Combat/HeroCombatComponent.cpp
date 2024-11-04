// Valerii Diachenko All Rights Reserved.


#include "Components/Combat/HeroCombatComponent.h"

#include "Weapons/WarriorHeroWeapon.h"

AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag GameplayTag) const
{
    return Cast<AWarriorHeroWeapon>(GetCharacterCarriedWeaponByTag(GameplayTag));
}