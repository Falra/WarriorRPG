// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorFunctionLibrary.generated.h"

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
};