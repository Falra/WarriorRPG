﻿// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "WarriorStructTypes.generated.h"

class UWarriorHeroLinkedAnimLayer;

USTRUCT(BlueprintType)
struct FWarriorHeroWeaponData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UWarriorHeroLinkedAnimLayer> WeaponLinkedAnimLayer;
};