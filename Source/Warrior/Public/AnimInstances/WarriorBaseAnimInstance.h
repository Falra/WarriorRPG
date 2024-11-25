// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WarriorBaseAnimInstance.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorBaseAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

protected:

    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (BlueprintThreadSafe))
    bool DoesOwnerHaveTag(FGameplayTag TagToCheck) const;
};