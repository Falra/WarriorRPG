﻿// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarriorBaseCharacter.h"
#include "WarriorHeroCharacter.generated.h"

struct FGameplayTag;
class UHeroCombatComponent;
struct FInputActionValue;
class UDataAsset_InputConfig;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class WARRIOR_API AWarriorHeroCharacter : public AWarriorBaseCharacter
{
    GENERATED_BODY()

public:
    AWarriorHeroCharacter();

    UHeroCombatComponent* GetCombatComponent() const { return CombatComponent.Get(); }
    
protected:

    virtual void BeginPlay() override;

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    //~ Begin APawn Interface.
    virtual void PossessedBy(AController* NewController) override;
    //~ End APawn Interface
    
private:

#pragma region Components

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> CameraBoom;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UHeroCombatComponent> CombatComponent;
    
#pragma endregion Components

#pragma region Inputs
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character data", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;
    
    void Input_Move(const FInputActionValue& InputActionValue);

    void Input_Look(const FInputActionValue& InputActionValue);

    void Input_AbilityInputPressed(FGameplayTag InputTag);
    
    void Input_AbilityInputReleased(FGameplayTag InputTag);
    
#pragma endregion Inputs

};