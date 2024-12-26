// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/WarriorAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "UI/HeroUIComponent.h"
#include "UI/PawnUIComponent.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
    InitCurrentHealth(1.f);
    InitMaxHealth(1.f);
    InitCurrentRage(1.f);
    InitMaxRage(1.f);
    InitAttackPower(1.f);
    InitDefensePower(1.f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (!CachedPawnUIInterface.IsValid())
    {
        CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
        checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"),
            *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
        
        CachedPawnUIComponent = CachedPawnUIInterface.IsValid() ? CachedPawnUIInterface->GetPawnUIComponent() : nullptr;

        checkf(CachedPawnUIComponent.IsValid(), TEXT("Couldn't extract a PawnUIComponent from %s"),
            *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
    }

    if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
    {
        const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
        SetCurrentHealth(NewCurrentHealth);

        if (CachedPawnUIComponent.IsValid())
        {
            CachedPawnUIComponent->OnCurrentHealthPercentChanged.Broadcast(NewCurrentHealth/GetMaxHealth());
        }
    }

    if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
    {
        const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
        SetCurrentRage(NewCurrentRage);

        if (GetCurrentRage() == GetMaxRage())
        {
            UWarriorFunctionLibrary::AddGameplayTagToActor(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Full);
        }
        else if (GetCurrentRage() == 0.f)
        {
            UWarriorFunctionLibrary::AddGameplayTagToActor(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_None);
        }
        else
        {
            UWarriorFunctionLibrary::RemoveGameplayTagFromActor(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Full);
            UWarriorFunctionLibrary::RemoveGameplayTagFromActor(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_None);
        }
        
        const auto* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent();
        if (IsValid(HeroUIComponent))
        {
            HeroUIComponent->OnCurrentRagePercentChanged.Broadcast(NewCurrentRage/GetMaxRage());
        }
    }

    if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
    {
        const float OldHealth = GetCurrentHealth();
        const float Damage = GetDamageTaken();

        const float NewCurrentHealth = FMath::Clamp(OldHealth - Damage, 0.f, GetMaxHealth());
        SetCurrentHealth(NewCurrentHealth);

        if (CachedPawnUIComponent.IsValid())
        {
            CachedPawnUIComponent->OnCurrentHealthPercentChanged.Broadcast(NewCurrentHealth/GetMaxHealth());
        }

        if (NewCurrentHealth == 0.f)
        {
            UWarriorFunctionLibrary::AddGameplayTagToActor(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Dead);
        }
    }
}