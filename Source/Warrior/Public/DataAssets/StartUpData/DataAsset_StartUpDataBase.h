// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UWarriorAbilitySystemComponent;
class UWarriorGameplayAbility;

/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_StartUpDataBase : public UDataAsset
{
    GENERATED_BODY()

public:

    virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASC, int32 ApplyLevel = 1);
    
protected:
    
    UPROPERTY(EditdefaultsOnly, Category = "StartUpData")
    TArray<TSubclassOf<UWarriorGameplayAbility>> ActivateOnGivenAbilities;

    UPROPERTY(EditdefaultsOnly, Category = "StartUpData")
    TArray<TSubclassOf<UWarriorGameplayAbility>> ReactiveAbilities;

    void GrantAbilities(UWarriorAbilitySystemComponent* InASC, const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilities, int32 ApplyLevel = 1);
};
