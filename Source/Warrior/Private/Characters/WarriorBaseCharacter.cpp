﻿// Valerii Diachenko All Rights Reserved.


#include "Characters/WarriorBaseCharacter.h"

#include "WarriorAbilitySystemComponent.h"
#include "WarriorAttributeSet.h"

AWarriorBaseCharacter::AWarriorBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    GetMesh()->bReceivesDecals = false;

    WarriorAbilitySystemComponent = CreateDefaultSubobject<UWarriorAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));

    WarriorAttributeSet = CreateDefaultSubobject<UWarriorAttributeSet>(TEXT("WarriorAttributeSet"));
}

UAbilitySystemComponent* AWarriorBaseCharacter::GetAbilitySystemComponent() const
{
    return GetWarriorAbilitySystemComponent();
}

void AWarriorBaseCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (IsValid(WarriorAbilitySystemComponent))
    {
        WarriorAbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}
