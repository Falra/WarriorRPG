// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarriorCharacterAnimInstance.h"
#include "WarriorHeroAnimInstance.generated.h"

class AWarriorHeroCharacter;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorHeroAnimInstance : public UWarriorCharacterAnimInstance
{
    GENERATED_BODY()

protected:
    virtual void NativeInitializeAnimation() override;

    virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
    TObjectPtr<AWarriorHeroCharacter> OwningHeroCharacter;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    bool bShouldEnterRelaxState;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    float EnterRelaxStateThreshold = 5.f;

    float IdleElapsedTime = 0.f;
};