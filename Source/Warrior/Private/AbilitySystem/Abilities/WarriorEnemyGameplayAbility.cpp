// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"

#include "WarriorEnemyCharacter.h"

AWarriorEnemyCharacter* UWarriorEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
    if (!CachedEnemyCharacter.IsValid())
    {
        CachedEnemyCharacter = Cast<AWarriorEnemyCharacter>(CurrentActorInfo->AvatarActor);
    }
    
    return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UWarriorEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
    const AWarriorEnemyCharacter* EnemyCharacter = GetEnemyCharacterFromActorInfo();
    return IsValid(EnemyCharacter) ? EnemyCharacter->GetCombatComponent() : nullptr;
}