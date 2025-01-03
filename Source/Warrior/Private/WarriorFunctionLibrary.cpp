// Valerii Diachenko All Rights Reserved.


#include "WarriorFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "WarriorAbilitySystemComponent.h"
#include "WarriorGameInstance.h"
#include "WarriorGameplayTags.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "WarriorTypes/WarriorCountDownAction.h"

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

UPawnCombatComponent* UWarriorFunctionLibrary::GetPawnCombatComponentFromActor_Native(AActor* Actor)
{
    if (!IsValid(Actor))
    {
        return nullptr;
    }

    const auto* ActorInterface = Cast<IPawnCombatInterface>(Actor);
    return ActorInterface ? ActorInterface->GetPawnCombatComponent() : nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::K2_GetPawnCombatComponentFromActor(AActor* Actor, bool& OutReturnValue)
{
    auto* CombatComponent = GetPawnCombatComponentFromActor_Native(Actor);
    OutReturnValue = !!CombatComponent;
    return CombatComponent;
}

bool UWarriorFunctionLibrary::IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn)
{
    check(QueryPawn && TargetPawn);
    
    const auto* QueryTeamAgent = Cast<const IGenericTeamAgentInterface>(QueryPawn->GetController());
    const auto* TargetTeamAgent = Cast<const IGenericTeamAgentInterface>(TargetPawn->GetController());
    if (QueryTeamAgent && TargetTeamAgent)
    {
        return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
    }
    return false;
}

float UWarriorFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
    return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UWarriorFunctionLibrary::ComputeHitReactDirectionTag(const AActor* InAttacker, const AActor* InVictim, double& OutAngleDifference)
{
    check(InAttacker && InVictim);

    const FVector VictimForward = InVictim->GetActorForwardVector();
    const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

    const double DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
    OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

    const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);

    if (CrossResult.Z < 0.f)
    {
        OutAngleDifference *= -1.f;
    }

    if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
    {
        return WarriorGameplayTags::Shared_Status_HitReact_Front;
    }

    if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f)
    {
        return WarriorGameplayTags::Shared_Status_HitReact_Left;
    }

    if (OutAngleDifference < -135.f || OutAngleDifference > 135.f)
    {
        return WarriorGameplayTags::Shared_Status_HitReact_Back;
    }

    if (OutAngleDifference > 45.f && OutAngleDifference <= 135.f)
    {
        return WarriorGameplayTags::Shared_Status_HitReact_Right;
    }
    
    return WarriorGameplayTags::Shared_Status_HitReact_Front;
}

bool UWarriorFunctionLibrary::IsValidBlock(const AActor* InAttacker, const AActor* InDefender)
{
    check(InAttacker && InDefender);

    const double DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

    const bool bValidBlock = DotResult < -0.1f;

    // const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"), DotResult,
    //     bValidBlock ? TEXT("Valid Block") : TEXT("InvalidBlock"));
    //
    // Debug::Print(DebugString, bValidBlock ? FColor::Green : FColor::Red);

    return bValidBlock;
}

bool UWarriorFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor,
    const FGameplayEffectSpecHandle& InSpecHandle)
{
    auto* SourceASC = GetWarriorASCFromActor_Native(InInstigator);
    auto* TargetASC = GetWarriorASCFromActor_Native(InTargetActor);

    const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
    return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UWarriorFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime,
    EWarriorCountDownActionInput CountDownInput, EWarriorCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
    UWorld* World = nullptr;

    if (GEngine)
    {
        World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    }

    if (!World)
    {
        return;
    }

    FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
    auto* FoundAction = LatentActionManager.FindExistingAction<FWarriorCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

    if (CountDownInput == EWarriorCountDownActionInput::Start)
    {
        if (!FoundAction)
        {
            LatentActionManager.AddNewAction(
                LatentInfo.CallbackTarget,
                LatentInfo.UUID,
                new FWarriorCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownOutput, LatentInfo)
                );
        }
    }

    if (CountDownInput == EWarriorCountDownActionInput::Cancel)
    {
        if (FoundAction)
        {
            FoundAction->CancelAction();
        }
    }
}

UWarriorGameInstance* UWarriorFunctionLibrary::GetWarriorGameInstance(const UObject* WorldContextObject)
{
    if (GEngine)
    {
        if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            return World->GetGameInstance<UWarriorGameInstance>();
        }
    }
    return nullptr;
}
