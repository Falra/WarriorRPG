// Valerii Diachenko All Rights Reserved.


#include "Items/Weapons/WarriorHeroWeapon.h"

void AWarriorHeroWeapon::AssignGrantedAbilityHandles(const TArray<FGameplayAbilitySpecHandle>& InAbilityHandles)
{
    GrantedAbilityHandles = InAbilityHandles;
}

TArray<FGameplayAbilitySpecHandle> AWarriorHeroWeapon::GetGrantedAbilityHandles() const
{
    return GrantedAbilityHandles;
}