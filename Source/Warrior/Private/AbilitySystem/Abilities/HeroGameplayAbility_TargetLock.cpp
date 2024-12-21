// Valerii Diachenko All Rights Reserved.


#include "Abilities/HeroGameplayAbility_TargetLock.h"

#include "EnhancedInputSubsystems.h"
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
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    TryLockOnTarget();
    InitTargetLockMovement();
    InitTargetLockMappingContext();
    
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    ResetTargetLockMovement();
    ResetTargetLockMappingContext();
    CleanUp();
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::SmoothLookAtTarget(float DeltaTime)
{
    const bool bShouldOverrideRotation =
        !UWarriorFunctionLibrary::DoesActorHaveTag_Native(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Rolling)
        &&
        !UWarriorFunctionLibrary::DoesActorHaveTag_Native(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Blocking);

    if (!bShouldOverrideRotation)
    {
        return;
    }

    FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
        GetHeroCharacterFromActorInfo()->GetActorLocation(),
        CurrentLockedActor->GetActorLocation()
        );

    LookAtRot -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);
    
    const FRotator CurrentControlRot = GetHeroControllerFromActorInfo()->GetControlRotation();
    const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

    GetHeroControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
    GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
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

    SmoothLookAtTarget(DeltaTime);
}


void UHeroGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
    GetAvailableActorsToLock();

    TArray<AActor*> ActorsOnLeft;
    TArray<AActor*> ActorsOnRight;
    AActor* NewTargetToLock;

    GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

    if (InSwitchDirectionTag == WarriorGameplayTags::Player_Event_SwitchTarget_Left)
    {
        NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
    }
    else
    {
        NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
    }

    if (NewTargetToLock)
    {
        CurrentLockedActor = NewTargetToLock;
    }
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
    AvailableActorsToLock.Empty();
    
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

void UHeroGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight)
{
    if (!CurrentLockedActor.IsValid() || AvailableActorsToLock.IsEmpty())
    {
        CancelTargetLockAbility();
        return;
    }
    
    const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
    const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

    for (AActor* AvailableActor : AvailableActorsToLock)
    {
        if(!AvailableActor || AvailableActor == CurrentLockedActor)
        {
            continue;
        }
        
        const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();
        const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

        if (CrossResult.Z > 0.f)
        {
            OutActorsOnRight.AddUnique(AvailableActor);
        }
        else
        {
            OutActorsOnLeft.AddUnique(AvailableActor);
        }
    }
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

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
    CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
    GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
    const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();

    auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    check(Subsystem)

    Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMappingContext()
{
    if (!GetHeroControllerFromActorInfo())
    {
        return;
    }

    const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();

    auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    check(Subsystem)

    Subsystem->RemoveMappingContext(TargetLockMappingContext);
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
    if (CachedDefaultMaxWalkSpeed > 0.f)
    {
        GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
    }
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

    CachedDefaultMaxWalkSpeed = 0.f;
}