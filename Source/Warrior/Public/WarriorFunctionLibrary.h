// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorFunctionLibrary.generated.h"

struct FGameplayEffectSpecHandle;
struct FScalableFloat;
class UPawnCombatComponent;
class UWarriorAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    static UWarriorAbilitySystemComponent* GetWarriorASCFromActor_Native(AActor* Actor);

    UFUNCTION(BlueprintCallable, Category="Warrior|Function Library")
    static void AddGameplayTagToActor(AActor* Actor, FGameplayTag Tag);

    UFUNCTION(BlueprintCallable, Category="Warrior|Function Library")
    static void RemoveGameplayTagFromActor(AActor* Actor, FGameplayTag Tag);

    static bool DoesActorHaveTag_Native(AActor* Actor, FGameplayTag Tag);

    UFUNCTION(BlueprintCallable, Category="Warrior|Function Library", meta = (DisplayName = "Does Actor Have Tag", ExpandBoolAsExecs = "ReturnValue"))
    static bool K2_DoesActorHaveTag(AActor* Actor, FGameplayTag Tag);

    static UPawnCombatComponent* GetPawnCombatComponentFromActor_Native(AActor* Actor);

    UFUNCTION(BlueprintCallable, Category="Warrior|Function Library", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandBoolAsExecs = "OutReturnValue"))
    static UPawnCombatComponent* K2_GetPawnCombatComponentFromActor(AActor* Actor, bool& OutReturnValue);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warrior|FunctionLibrary")
    static bool IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn);

    UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
    static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel = 1.f);

    UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
    static FGameplayTag ComputeHitReactDirectionTag(const AActor* InAttacker, const AActor* InVictim, double& OutAngleDifference);

    UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
    static bool IsValidBlock(const AActor* InAttacker, const AActor* InDefender);

    UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
    static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle);
};