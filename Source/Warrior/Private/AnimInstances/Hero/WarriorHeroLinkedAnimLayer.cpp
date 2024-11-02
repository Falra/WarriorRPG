// Valerii Diachenko All Rights Reserved.


#include "Hero/WarriorHeroLinkedAnimLayer.h"

#include "Hero/WarriorHeroAnimInstance.h"

UWarriorHeroAnimInstance* UWarriorHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
    return Cast<UWarriorHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}