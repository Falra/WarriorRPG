// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WarriorGameplayAbility.generated.h"

class UWarriorAbilitySystemComponent;
class UPawnCombatComponent;

UENUM(BlueprintType)
enum class EWarriorAbilityActivationPolicy : uint8
{
    OnTriggered,
    OnGiven
};

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

protected:

    //~ Begin UGameplayAbility Interface.
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    //~ End UGameplayAbility Interface

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|Ability")
    UPawnCombatComponent* GetCombatComponentFromActorInfo() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|Ability")
    UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponentFromActorInfo() const;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Warrior|Ability")
    EWarriorAbilityActivationPolicy AbilityActivationPolicy = EWarriorAbilityActivationPolicy::OnTriggered;

    FActiveGameplayEffectHandle ApplyEffectSpecHandleToTarget_Native(AActor* TargetActor, const FGameplayEffectSpecHandle& EffectSpecHandle);

    UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (DisplayName = "Apply Effect Spec Handle To Target", ExpandBoolAsExecs = "OutSuccess"))
    FActiveGameplayEffectHandle K2_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& EffectSpecHandle, bool& OutSuccess);

    UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
    void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults);
};