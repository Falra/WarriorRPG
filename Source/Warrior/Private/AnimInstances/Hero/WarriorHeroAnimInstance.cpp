// Valerii Diachenko All Rights Reserved.


#include "Hero/WarriorHeroAnimInstance.h"

#include "WarriorHeroCharacter.h"

void UWarriorHeroAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    if (IsValid(OwningCharacter))
    {
        OwningHeroCharacter = Cast<AWarriorHeroCharacter>(OwningCharacter);
    }
}

void UWarriorHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

    if (bHasAcceleration)
    {
        IdleElapsedTime = 0.f;
        bShouldEnterRelaxState = false;
    }
    else
    {
        IdleElapsedTime += DeltaSeconds;
        bShouldEnterRelaxState = IdleElapsedTime >= EnterRelaxStateThreshold;
    }
}