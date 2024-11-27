// Valerii Diachenko All Rights Reserved.


#include "AI/BTTask_RotateToFaceTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
    NodeName = TEXT("Native Rotate to Face Target Actor");
    AnglePrecision = 10.f;
    RotationInterpSpeed = 5.f;

    bNotifyTick = true;
    bNotifyTaskFinished = true;
    bCreateNodeInstance = true;

    INIT_TASK_NODE_NOTIFY_FLAGS();

    TargetFaceKey.AddObjectFilter(this,GET_MEMBER_NAME_CHECKED(ThisClass, TargetFaceKey), AActor::StaticClass());
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);

    if (const UBlackboardData* BBAsset = GetBlackboardAsset())
    {
        TargetFaceKey.ResolveSelectedKey(*BBAsset);
    }
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
    return sizeof(FRotateToFaceTargetTaskMemory);
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
    const FString KeyDescription = TargetFaceKey.SelectedKeyName.ToString();
    return FString::Printf(TEXT("Smoothly rotates to face %s Key until the angle precision: %s is reached"), *KeyDescription,
        *FString::SanitizeFloat(AnglePrecision));
}

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetFaceKey.SelectedKeyName);
    auto* TargetActor = Cast<AActor>(ActorObject);

    APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

    auto* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
    check(Memory);

    Memory->OwningPawn = OwningPawn;
    Memory->TargetActor = TargetActor;

    if (!Memory->IsValid())
    {
        return EBTNodeResult::Failed;
    }

    if (HasReachedAnglePrecision(OwningPawn, TargetActor))
    {
        Memory->Reset();
        return EBTNodeResult::Succeeded;
    }
    
    return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    auto* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
    if (!Memory->IsValid())
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    if (HasReachedAnglePrecision(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
    {
        Memory->Reset();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Memory->OwningPawn->GetActorLocation(),
            Memory->TargetActor->GetActorLocation());

        const FRotator TargetRot = FMath::RInterpTo(Memory->OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);

        Memory->OwningPawn->SetActorRotation(TargetRot);
    }
}

bool UBTTask_RotateToFaceTarget::HasReachedAnglePrecision(const APawn* QueryPawn, const AActor* TargetActor) const
{
    const FVector OwnerForward = QueryPawn->GetActorForwardVector();
    const FVector OwnerToTargetNormalized = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

    const double DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalized);
    const double AngleDiff = UKismetMathLibrary::DegAcos(DotResult);

    return AngleDiff <= AnglePrecision;
}