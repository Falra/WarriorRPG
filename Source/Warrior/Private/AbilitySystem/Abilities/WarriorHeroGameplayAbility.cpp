// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

#include "WarriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"
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

FGameplayEffectSpecHandle UWarriorHeroGameplayAbility::MakeHeroDamageSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,
    float WeaponBaseDamage, FGameplayTag AttackTypeTag, int32 ComboCount)
{
    check(EffectClass);

    auto EffectContext = GetWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    EffectContext.SetAbility(this);
    auto* AvatarActor = GetAvatarActorFromActorInfo();
    EffectContext.AddSourceObject(AvatarActor);
    EffectContext.AddInstigator(AvatarActor, AvatarActor);
    
    auto EffectSpecHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        EffectContext
    );

    EffectSpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_BaseDamage, WeaponBaseDamage);
    
    if (AttackTypeTag.IsValid())
    {
        EffectSpecHandle.Data->SetSetByCallerMagnitude(AttackTypeTag, ComboCount);
    }
    
    return EffectSpecHandle;
}

UHeroUIComponent* UWarriorHeroGameplayAbility::GetHeroUIComponentFromActorInfo()
{
    return GetHeroCharacterFromActorInfo()->GetHeroUIComponent();
}

bool UWarriorHeroGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime)
{
    check(InCooldownTag.IsValid());

    const FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
    TArray<TPair<float, float>> TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);
    if (!TimeRemainingAndDuration.IsEmpty())
    {
        RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
        TotalCooldownTime = TimeRemainingAndDuration[0].Value;
    }
    return RemainingCooldownTime > 0.f;
}