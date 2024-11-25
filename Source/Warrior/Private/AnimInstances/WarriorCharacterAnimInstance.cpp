// Valerii Diachenko All Rights Reserved.


#include "WarriorCharacterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "WarriorBaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    OwningCharacter = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());

    if (IsValid(OwningCharacter))
    {
        OwningCharacterMovement = OwningCharacter->GetCharacterMovement();
    }
}

void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

    if (IsValid(OwningCharacter))
    {
        GroundSpeed = OwningCharacter->GetVelocity().Size2D();
        LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(),OwningCharacter->GetActorRotation());
    }

    if (IsValid(OwningCharacterMovement))
    {
        bHasAcceleration = OwningCharacterMovement->GetCurrentAcceleration().SizeSquared2D() > 0.f;
    }
}