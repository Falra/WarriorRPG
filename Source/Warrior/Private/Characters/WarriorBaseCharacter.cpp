// Valerii Diachenko All Rights Reserved.


#include "Characters/WarriorBaseCharacter.h"

AWarriorBaseCharacter::AWarriorBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    GetMesh()->bReceivesDecals = false;
}
