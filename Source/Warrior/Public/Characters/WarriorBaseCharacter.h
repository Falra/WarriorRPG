// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "WarriorBaseCharacter.generated.h"

class UWarriorAttributeSet;
class UWarriorAbilitySystemComponent;

UCLASS()
class WARRIOR_API AWarriorBaseCharacter : public ACharacter,  public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AWarriorBaseCharacter();

    //~ Begin IAbilitySystemInterface Interface.
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    //~ End IAbilitySystemInterface Interface

protected:

    //~ Begin APawn Interface.
    virtual void PossessedBy(AController* NewController) override;
    //~ End APawn Interface
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability system")
    TObjectPtr<UWarriorAbilitySystemComponent> WarriorAbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability system")
    TObjectPtr<UWarriorAttributeSet> WarriorAttributeSet;

public:
    FORCEINLINE_DEBUGGABLE UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponent() const { return WarriorAbilitySystemComponent; }

    FORCEINLINE_DEBUGGABLE UWarriorAttributeSet* GetWarriorAttributeSet() const { return WarriorAttributeSet; }
};