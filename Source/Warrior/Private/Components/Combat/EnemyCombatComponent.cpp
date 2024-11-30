// Valerii Diachenko All Rights Reserved.


#include "Components/Combat/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"


void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    Super::OnHitTargetActor(HitActor);

    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }

    OverlappedActors.AddUnique(HitActor);

    // TODO: Implement block check
    bool bIsValidBlock = false;

    const bool bIsPlayerBlocking = UWarriorFunctionLibrary::DoesActorHaveTag_Native(HitActor, WarriorGameplayTags::Player_Status_Blocking);
    const bool bIsMyAttackUnblockable = false;
    if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
    {
        bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
    }

    FGameplayEventData EventData;
    EventData.Instigator = GetOwningPawn();
    EventData.Target = HitActor;
    if (bIsValidBlock)
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            HitActor,
            WarriorGameplayTags::Player_Event_SuccessfulBlock,
            EventData
        );
    }
    else
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            GetOwningPawn(),
            WarriorGameplayTags::Shared_Event_MeleeHit,
            EventData
            );
    }
}