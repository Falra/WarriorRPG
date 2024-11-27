// Valerii Diachenko All Rights Reserved.


#include "AI/BTTask_RotateToFaceTarget.h"

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