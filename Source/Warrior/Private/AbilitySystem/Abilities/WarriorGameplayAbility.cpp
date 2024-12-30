// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Combat/PawnCombatComponent.h"

void UWarriorGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    if (AbilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven && ActorInfo && !Spec.IsActive())
    {
        ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
    }
}

void UWarriorGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (AbilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven && ActorInfo)
    {
        ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
    }
}

UPawnCombatComponent* UWarriorGameplayAbility::GetCombatComponentFromActorInfo() const
{
    return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UWarriorAbilitySystemComponent* UWarriorGameplayAbility::GetWarriorAbilitySystemComponentFromActorInfo() const
{
    return Cast<UWarriorAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UWarriorGameplayAbility::ApplyEffectSpecHandleToTarget_Native(AActor* TargetActor,
    const FGameplayEffectSpecHandle& EffectSpecHandle)
{
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    check(TargetASC && EffectSpecHandle.IsValid());
    
    return GetWarriorAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *EffectSpecHandle.Data,
        TargetASC
        );
}

FActiveGameplayEffectHandle UWarriorGameplayAbility::K2_ApplyEffectSpecHandleToTarget(AActor* TargetActor,
    const FGameplayEffectSpecHandle& EffectSpecHandle, bool& OutSuccess)
{
    const auto EffectHandle = ApplyEffectSpecHandleToTarget_Native(TargetActor, EffectSpecHandle);
    OutSuccess = EffectHandle.WasSuccessfullyApplied();
    return EffectHandle;
}

void UWarriorGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults)
{
    if (InHitResults.IsEmpty())
    {
        return;
    }

    auto* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());
    for (const FHitResult& Hit : InHitResults)
    {
        auto* HitPawn = Cast<APawn>(Hit.GetActor());
        
        if (!HitPawn)
        {
            continue;
        }
        
        if (!UWarriorFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
        {
            continue;
        }
        
        FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectSpecHandleToTarget_Native(HitPawn, InSpecHandle);
        if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
        {
            FGameplayEventData Data;
            Data.Instigator = OwningPawn;
            Data.Target = HitPawn;

            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
                HitPawn,
                WarriorGameplayTags::Shared_Event_HitReact,
                Data
                );
        }
    }
}