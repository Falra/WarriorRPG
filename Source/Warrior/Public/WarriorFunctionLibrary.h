// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorFunctionLibrary.generated.h"

class UWarriorGameInstance;
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

    UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0", UpdateInterval = "0.1"))
    static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EWarriorCountDownActionInput CountDownInput,
        UPARAM(DisplayName = "Output") EWarriorCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);

    UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
    static UWarriorGameInstance* GetWarriorGameInstance(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable,Category = "Warrior|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
    static void ToggleInputMode(const UObject* WorldContextObject, EWarriorInputMode InInputMode);

    UFUNCTION(BlueprintCallable,Category = "Warrior|FunctionLibrary")
    static void SaveCurrentGameDifficulty(EWarriorGameDifficulty InDifficultyToSave);

    UFUNCTION(BlueprintCallable,Category = "Warrior|FunctionLibrary")
    static bool TryLoadSavedGameDifficulty(EWarriorGameDifficulty& OutSavedDifficulty);
};