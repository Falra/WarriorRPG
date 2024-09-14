// Valerii Diachenko All Rights Reserved.


#include "DataAssets/Input/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
    for (const auto& [InputTag, InputAction] : NativeInputActions)
    {
        if (InputTag == InInputTag && InputAction)
        {
            return InputAction;
        }
    }
    
    return nullptr;
}