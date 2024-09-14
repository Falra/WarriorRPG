// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_InputConfig.generated.h"

class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FWarriorInputActionConfig
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
    FGameplayTag InputTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UInputAction> InputAction;
};

/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_InputConfig : public UDataAsset
{
    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
    TArray<FWarriorInputActionConfig> NativeInputActions;

    UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;
};
