// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PawnUIComponent.h"
#include "HeroUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdatedDelegate, FGameplayTag, AbilityInputTag, TSoftObjectPtr<UMaterialInterface>, SoftAbilityIconMaterial);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WARRIOR_API UHeroUIComponent : public UPawnUIComponent
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintAssignable)
    FOnPercentChangedDelegate OnCurrentRagePercentChanged;

    UPROPERTY(BlueprintCallable, BlueprintAssignable)
    FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;

    UPROPERTY(BlueprintCallable, BlueprintAssignable)
    FOnAbilityIconSlotUpdatedDelegate OnAbilityIconSlotUpdated;
};