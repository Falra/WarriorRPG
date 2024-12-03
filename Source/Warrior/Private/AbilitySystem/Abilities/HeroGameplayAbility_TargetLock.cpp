// Valerii Diachenko All Rights Reserved.


#include "Abilities/HeroGameplayAbility_TargetLock.h"

#include "WarriorDebugHelper.h"
#include "WarriorHeroCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    TryLockOnTarget();
    
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
    GetAvailableActorsToLock();
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
    TArray<FHitResult> BoxTraceHits;
    const AWarriorHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();
    const FVector Start = HeroCharacter->GetActorLocation();
    const FVector ForwardVector = HeroCharacter->GetActorForwardVector();

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetHeroCharacterFromActorInfo());
    
    UKismetSystemLibrary::BoxTraceMultiForObjects(
        HeroCharacter,
        Start,
        Start + ForwardVector * BoxTraceDistance,
        TraceBoxSize / 2.f,
        ForwardVector.ToOrientationRotator(),
        BoxTraceChannel,
        false,
        ActorsToIgnore,
        bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
        BoxTraceHits,
        true
        );
    
    for (const FHitResult& TraceHit : BoxTraceHits)
    {
        if (AActor* HitActor = TraceHit.GetActor())
        {
            if (HitActor != HeroCharacter)
            {
                AvailableActorsToLock.AddUnique(HitActor);
                Debug::Print(HitActor->GetActorNameOrLabel());
            }
        }
    }
}