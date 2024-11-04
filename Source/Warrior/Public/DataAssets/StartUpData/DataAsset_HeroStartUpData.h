// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_HeroStartUpData.generated.h"

struct FWarriorHeroAbilitySet;

/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_HeroStartUpData : public UDataAsset_StartUpDataBase
{
    GENERATED_BODY()

public:

    virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASC, int32 ApplyLevel = 1) override;

private:
    UPROPERTY(EditdefaultsOnly, Category = "StartUpData",  meta = (TitleProperty = "InputTag"))
    TArray<FWarriorHeroAbilitySet> HeroStartUpAbilities;
};