﻿// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "WarriorBaseCharacter.generated.h"

class UDataAsset_StartUpDataBase;
class UWarriorAttributeSet;
class UWarriorAbilitySystemComponent;
class UMotionWarpingComponent;

UCLASS()
class WARRIOR_API AWarriorBaseCharacter : public ACharacter,  public IAbilitySystemInterface, public IPawnCombatInterface, public IPawnUIInterface
{
    GENERATED_BODY()

public:
    AWarriorBaseCharacter();

    //~ Begin IAbilitySystemInterface Interface.
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    //~ End IAbilitySystemInterface Interface

    //~ Begin IPawnCombatInterface Interface.
    virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
    //~ End IPawnCombatInterface Interface

    //~ Begin IPawnUIInterface Interface.
    virtual UPawnUIComponent* GetPawnUIComponent() const override;
    //~ End IPawnUIInterface Interface

protected:

    //~ Begin APawn Interface.
    virtual void PossessedBy(AController* NewController) override;
    //~ End APawn Interface
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability system")
    TObjectPtr<UWarriorAbilitySystemComponent> WarriorAbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Warping")
    TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability system")
    TObjectPtr<UWarriorAttributeSet> WarriorAttributeSet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character data")
    TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

public:
    FORCEINLINE_DEBUGGABLE UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponent() const { return WarriorAbilitySystemComponent; }

    FORCEINLINE_DEBUGGABLE UWarriorAttributeSet* GetWarriorAttributeSet() const { return WarriorAttributeSet; }
};