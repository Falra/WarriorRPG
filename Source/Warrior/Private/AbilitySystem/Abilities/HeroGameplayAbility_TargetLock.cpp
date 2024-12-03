// Valerii Diachenko All Rights Reserved.


#include "Abilities/HeroGameplayAbility_TargetLock.h"

#include "WarriorDebugHelper.h"
#include "WarriorHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
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
    CleanUp();
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
    GetAvailableActorsToLock();

    if (AvailableActorsToLock.IsEmpty())
    {
        CancelTargetLockAbility();
        return;
    }
    
    CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);
    if (CurrentLockedActor.IsValid())
    {
        Debug::Print(CurrentLockedActor->GetActorNameOrLabel());
    }
    else
    {
        CancelTargetLockAbility();
    }
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
            }
        }
    }
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& AvailableActors)
{
    float ClosestDistance = 0.f;

    return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), AvailableActors, ClosestDistance);
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
    CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
    AvailableActorsToLock.Empty();
    CurrentLockedActor = nullptr;
}