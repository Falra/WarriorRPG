// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

#include "WarriorHeroCharacter.h"
#include "WarriorHeroController.h"

AWarriorHeroCharacter* UWarriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
    if (!CachedHeroCharacter.IsValid())
    {
        CachedHeroCharacter = Cast<AWarriorHeroCharacter>(CurrentActorInfo->AvatarActor);
    }
    
    return CachedHeroCharacter.IsValid() ? CachedHeroCharacter.Get() : nullptr;
}

AWarriorHeroController* UWarriorHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
    if (!CachedHeroController.IsValid())
    {
        CachedHeroController = Cast<AWarriorHeroController>(CurrentActorInfo->PlayerController);
    }
    
    return CachedHeroController.IsValid() ? CachedHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWarriorHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
    const AWarriorHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();
    return IsValid(HeroCharacter) ? HeroCharacter->GetCombatComponent() : nullptr;
}