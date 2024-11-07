// Valerii Diachenko All Rights Reserved.


#include "WarriorFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorAbilitySystemComponent.h"

UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::GetWarriorASCFromActor_Native(AActor* Actor)
{
    if (!IsValid(Actor))
    {
        return nullptr;
    }
    
    return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
}

void UWarriorFunctionLibrary::AddGameplayTagToActor(AActor* Actor, FGameplayTag Tag)
{
    auto* ASC = GetWarriorASCFromActor_Native(Actor);
    if (IsValid(ASC) && !ASC->HasMatchingGameplayTag(Tag))
    {
        ASC->AddLooseGameplayTag(Tag);
    }
}

void UWarriorFunctionLibrary::RemoveGameplayTagFromActor(AActor* Actor, FGameplayTag Tag)
{
    auto* ASC = GetWarriorASCFromActor_Native(Actor);
    if (IsValid(ASC) && ASC->HasMatchingGameplayTag(Tag))
    {
        ASC->RemoveLooseGameplayTag(Tag);
    }
}

bool UWarriorFunctionLibrary::DoesActorHaveTag_Native(AActor* Actor, FGameplayTag Tag)
{
    const auto* ASC = GetWarriorASCFromActor_Native(Actor);
    return IsValid(ASC) && ASC->HasMatchingGameplayTag(Tag);
}

bool UWarriorFunctionLibrary::K2_DoesActorHaveTag(AActor* Actor, FGameplayTag Tag)
{
    return DoesActorHaveTag_Native(Actor, Tag);
}