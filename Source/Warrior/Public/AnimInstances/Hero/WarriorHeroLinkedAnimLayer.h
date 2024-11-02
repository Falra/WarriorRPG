// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarriorBaseAnimInstance.h"
#include "WarriorHeroLinkedAnimLayer.generated.h"

class UWarriorHeroAnimInstance;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorHeroLinkedAnimLayer : public UWarriorBaseAnimInstance
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintPure, BlueprintCallable, meta=(BlueprintThreadSafe))
    UWarriorHeroAnimInstance* GetHeroAnimInstance() const;
};