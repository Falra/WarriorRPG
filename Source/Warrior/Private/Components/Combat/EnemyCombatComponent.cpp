// Valerii Diachenko All Rights Reserved.


#include "Components/Combat/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorEnemyCharacter.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Components/BoxComponent.h"


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
    const bool bIsMyAttackUnblockable = UWarriorFunctionLibrary::DoesActorHaveTag_Native(GetOwningPawn(), WarriorGameplayTags::Enemy_Status_Unbloackable);

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

void UEnemyCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
    Super::ToggleBodyCollisionBoxCollision(bShouldEnable, ToggleDamageType);

    const auto* OwningEnemyCharacter = GetOwningPawn<AWarriorEnemyCharacter>();

    check(OwningEnemyCharacter);

    UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
    UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();

    check(LeftHandCollisionBox && RightHandCollisionBox);

    switch (ToggleDamageType)
    {
        case EToggleDamageType::LeftHand:
            LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
            break;
        case EToggleDamageType::RightHand:
            RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
            break;
        case EToggleDamageType::CurrentEquipWeapon:
        default:
            break;
    }
    
    if (!bShouldEnable)
    {
        OverlappedActors.Empty();
    }
}

