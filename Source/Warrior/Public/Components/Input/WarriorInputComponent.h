// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/DataAsset_InputConfig.h"
#include "WarriorInputComponent.generated.h"


struct FGameplayTag;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WARRIOR_API UWarriorInputComponent : public UEnhancedInputComponent
{
    GENERATED_BODY()

public:

    template<class UserObject, typename CallbackFunc>
    void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Callback);

    template<class UserObject, typename CallbackFunc>
    void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
};

template <class UserObject, typename CallbackFunc>
void UWarriorInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag,
    ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Callback)
{
    check(InInputConfig);

    if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
    {
        BindAction(FoundAction, TriggerEvent, ContextObject, Callback);
    }
}

template <class UserObject, typename CallbackFunc>
void UWarriorInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject,
    CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
    check(InInputConfig);

    for (const FWarriorInputActionConfig& InputActionConfig: InInputConfig->AbilityInputActions)
    {
        if (!InputActionConfig.IsValid())
        {
            continue;
        }
        BindAction(InputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, InputActionConfig.InputTag);
        BindAction(InputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, InputActionConfig.InputTag);
    }
}