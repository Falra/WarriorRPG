// Valerii Diachenko All Rights Reserved.


#include "Abilities/HeroGameplayAbility_PickUpStones.h"

#include "WarriorHeroCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PickUps/WarriorStoneBase.h"
#include "UI/HeroUIComponent.h"

void UHeroGameplayAbility_PickUpStones::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(true);
    
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_PickUpStones::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(false);
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_PickUpStones::CollectStones()
{
    CollectedStones.Empty();

    TArray<FHitResult> TraceHits;

    const AWarriorHeroCharacter* Hero = GetHeroCharacterFromActorInfo();
    const FVector HeroLocation = Hero->GetActorLocation();
    const FVector HeroDownVector = -Hero->GetActorUpVector();

    UKismetSystemLibrary::BoxTraceMultiForObjects(
        Hero,
        HeroLocation,
        HeroLocation + HeroDownVector * BoxTraceDistance,
        TraceBoxSize / 2.f,
        HeroDownVector.ToOrientationRotator(),
        StoneTraceChannel,
        false,
        TArray<AActor*>(),
        bDrawDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
        TraceHits,
        true
    );

    for (const FHitResult& TraceHit : TraceHits)
    {
        if (auto* FoundStone = Cast<AWarriorStoneBase>(TraceHit.GetActor()))
        {
            CollectedStones.AddUnique(FoundStone);
        }
    }
    
    if (CollectedStones.IsEmpty())
    {
        CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
    }
}

void UHeroGameplayAbility_PickUpStones::ConsumeStones()
{
    if (CollectedStones.IsEmpty())
    {
        CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
        return;
    }

    UWarriorAbilitySystemComponent* AbilitySystemComponent = GetWarriorAbilitySystemComponentFromActorInfo();
    const int32 ApplyLevel = GetAbilityLevel();
    
    for (AWarriorStoneBase* CollectedStone : CollectedStones)
    {
        if (CollectedStone)
        {
            CollectedStone->Consume(AbilitySystemComponent, ApplyLevel);
        }
    }
}