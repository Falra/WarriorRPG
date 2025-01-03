// Valerii Diachenko All Rights Reserved.


#include "Components/Combat/PawnCombatComponent.h"

#include "Components/BoxComponent.h"
#include "Weapons/WarriorWeaponBase.h"
#include "WarriorDebugHelper.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, AWarriorWeaponBase* Weapon, bool bEquipped)
{
    checkf(!CharacterCarriedWeaponMap.Contains(WeaponTag), TEXT("A weapon named %s has already been added as carried weapon"), *WeaponTag.ToString());
    check(Weapon);

    CharacterCarriedWeaponMap.Add(WeaponTag, Weapon);

    Weapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
    Weapon->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

    if (bEquipped)
    {
        EquippedWeaponTag = WeaponTag;
    }
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
    if (CharacterCarriedWeaponMap.Contains(WeaponTag))
    {
        return *CharacterCarriedWeaponMap.Find(WeaponTag);
    }
    
    return nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterEquippedWeapon() const
{
    if (!EquippedWeaponTag.IsValid())
    {
        return nullptr;
    }
    
    return GetCharacterCarriedWeaponByTag(EquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bEnable, EToggleDamageType ToggleDamageType)
{
    if (ToggleDamageType == EToggleDamageType::CurrentEquipWeapon)
    {
        ToggleCurrentEquippedWeaponCollision(bEnable);
    }
    else
    {
        ToggleBodyCollisionBoxCollision(bEnable, ToggleDamageType);
    }
}

void UPawnCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
{
    AWarriorWeaponBase* WeaponToToggle = GetCharacterEquippedWeapon();

    check(WeaponToToggle);

    if (bShouldEnable)
    {
        WeaponToToggle->GetWeaponCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
    else
    {
        WeaponToToggle->GetWeaponCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        OverlappedActors.Empty();
    }
}

void UPawnCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType) {}