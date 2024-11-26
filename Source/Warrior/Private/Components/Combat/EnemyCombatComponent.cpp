// Valerii Diachenko All Rights Reserved.


#include "Components/Combat/EnemyCombatComponent.h"

#include "WarriorDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    Super::OnHitTargetActor(HitActor);

    if (HitActor)
    {
        Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT(" is hitting ") + HitActor->GetActorNameOrLabel());
    }
}