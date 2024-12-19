// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UInputMappingContext;
class UWarriorWidgetBase;
/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroGameplayAbility_TargetLock : public UWarriorHeroGameplayAbility
{
    GENERATED_BODY()

protected:
    //~ Begin UGameplayAbility Interface
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    void SmoothLookAtTarget(float DeltaTime);
    //~ End UGameplayAbility Interface

    UFUNCTION(BlueprintCallable)
    void OnTargetLockTick(float DeltaTime);

    UFUNCTION(BlueprintCallable)
    void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);

private:

    void TryLockOnTarget();

    void GetAvailableActorsToLock();

    AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& AvailableActors);

    void DrawTargetLockWidget();

    void SetTargetLockWidgetPosition();

    void InitTargetLockMovement();

    void InitTargetLockMappingContext();
    
    void CancelTargetLockAbility();

    void CleanUp();

    void ResetTargetLockMovement();

    void ResetTargetLockMappingContext();

    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    TSubclassOf<UWarriorWidgetBase> TargetLockWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    float TargetLockRotationInterpSpeed = 5.f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    float TargetLockMaxWalkSpeed = 150.f;

    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    UInputMappingContext* TargetLockMappingContext;
    
    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    float BoxTraceDistance = 5000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

    UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
    bool bShowPersistentDebugShape = false;

    UPROPERTY(Transient)
    TArray<AActor*> AvailableActorsToLock;

    UPROPERTY(Transient)
    TWeakObjectPtr<AActor> CurrentLockedActor;

    UPROPERTY(Transient)
    UWarriorWidgetBase* DrawnTargetLockWidget;

    UPROPERTY(Transient)
    FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

    UPROPERTY()
    float CachedDefaultMaxWalkSpeed = 0.f;

};
