// Valerii Diachenko All Rights Reserved.


#include "AnimInstances/WarriorBaseAnimInstance.h"

#include "WarriorFunctionLibrary.h"

bool UWarriorBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
    if (APawn* OwningPawn = TryGetPawnOwner())
    {
        return UWarriorFunctionLibrary::DoesActorHaveTag_Native(OwningPawn, TagToCheck);
    }
    return false;
}