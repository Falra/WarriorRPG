// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"

class AWarriorWeaponBase;
/**
 * 
 */
UCLASS()
class WARRIOR_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
    void RegisterSpawnedWeapon(FGameplayTag WeaponTag, AWarriorWeaponBase* Weapon, bool bEquipped = false);

    UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
    AWarriorWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const;

    UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat")
    FGameplayTag EquippedWeaponTag;

    UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
    AWarriorWeaponBase* GetCharacterEquippedWeapon() const;

private:
    
    TMap<FGameplayTag, AWarriorWeaponBase*> CharacterCarriedWeaponMap;
    
};