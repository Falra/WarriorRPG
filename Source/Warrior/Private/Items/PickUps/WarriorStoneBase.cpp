// Valerii Diachenko All Rights Reserved.


#include "PickUps/WarriorStoneBase.h"

#include "WarriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"
#include "WarriorHeroCharacter.h"

void AWarriorStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnPickUpCollisionSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if (const auto* OverlappedHeroCharacter = Cast<AWarriorHeroCharacter>(OtherActor))
    {
        OverlappedHeroCharacter->GetWarriorAbilitySystemComponent()->TryActivateAbilityByTag(WarriorGameplayTags::Player_Ability_PickUp_Stones);
    }
}