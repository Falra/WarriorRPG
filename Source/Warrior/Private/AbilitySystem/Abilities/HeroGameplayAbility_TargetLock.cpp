// Valerii Diachenko All Rights Reserved.


#include "Abilities/HeroGameplayAbility_TargetLock.h"

#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "WarriorHeroCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/WarriorWidgetBase.h"
#include "Controllers/WarriorHeroController.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    TryLockOnTarget();
    
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    CleanUp();
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
    if (!CurrentLockedActor.IsValid() ||
        UWarriorFunctionLibrary::DoesActorHaveTag_Native(CurrentLockedActor.Get(), WarriorGameplayTags::Shared_Status_Dead) ||
        UWarriorFunctionLibrary::DoesActorHaveTag_Native(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Shared_Status_Dead)
    )
    {
        CancelTargetLockAbility();
        return;
    }
    
    SetTargetLockWidgetPosition();
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
    GetAvailableActorsToLock();

    if (AvailableActorsToLock.IsEmpty())
    {
        CancelTargetLockAbility();
        return;
    }
    
    CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);
    if (CurrentLockedActor.IsValid())
    {
        DrawTargetLockWidget();

        SetTargetLockWidgetPosition();
    }
    else
    {
        CancelTargetLockAbility();
    }
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
    TArray<FHitResult> BoxTraceHits;
    const AWarriorHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();
    const FVector Start = HeroCharacter->GetActorLocation();
    const FVector ForwardVector = HeroCharacter->GetActorForwardVector();

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetHeroCharacterFromActorInfo());
    
    UKismetSystemLibrary::BoxTraceMultiForObjects(
        HeroCharacter,
        Start,
        Start + ForwardVector * BoxTraceDistance,
        TraceBoxSize / 2.f,
        ForwardVector.ToOrientationRotator(),
        BoxTraceChannel,
        false,
        ActorsToIgnore,
        bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
        BoxTraceHits,
        true
        );
    
    for (const FHitResult& TraceHit : BoxTraceHits)
    {
        if (AActor* HitActor = TraceHit.GetActor())
        {
            if (HitActor != HeroCharacter)
            {
                AvailableActorsToLock.AddUnique(HitActor);
            }
        }
    }
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& AvailableActors)
{
    float ClosestDistance = 0.f;

    return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), AvailableActors, ClosestDistance);
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
    if (!DrawnTargetLockWidget)
    {
        checkf(TargetLockWidgetClass, TEXT("Forgot to assign a valid widget class in Blueprint"));
        DrawnTargetLockWidget = CreateWidget<UWarriorWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);
        check(DrawnTargetLockWidget);
        DrawnTargetLockWidget->AddToViewport();
    }
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
    if (!DrawnTargetLockWidget || !CurrentLockedActor.IsValid())
    {
        CancelTargetLockAbility();
        return;
    }

    FVector2D ScreenPosition;
    UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
        GetHeroControllerFromActorInfo(),
        CurrentLockedActor->GetActorLocation(),
        ScreenPosition,
        true
        );
    
    if (TargetLockWidgetSize == FVector2D::ZeroVector)
    {
        DrawnTargetLockWidget->WidgetTree->ForEachWidget(
            [this](UWidget* FoundWidget)
            {
                if (const auto* FoundSizeBox = Cast<USizeBox>(FoundWidget))
                {
                    TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
                    TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
                }
            }
            );
    }
    ScreenPosition -= (TargetLockWidgetSize / 2.f);
    DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
    CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
    AvailableActorsToLock.Empty();
    CurrentLockedActor = nullptr;

    if (DrawnTargetLockWidget)
    {
        DrawnTargetLockWidget->RemoveFromParent();
    }

    DrawnTargetLockWidget = nullptr;
    
    TargetLockWidgetSize = FVector2D::ZeroVector;
}