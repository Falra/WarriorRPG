// Valerii Diachenko All Rights Reserved.


#include "AbilityTasks/AbilityTask_ExecuteTaskOnTick.h"

UAbilityTask_ExecuteTaskOnTick::UAbilityTask_ExecuteTaskOnTick()
{
    bTickingTask = true;
}

UAbilityTask_ExecuteTaskOnTick* UAbilityTask_ExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
    return NewAbilityTask<UAbilityTask_ExecuteTaskOnTick>(OwningAbility);
}

void UAbilityTask_ExecuteTaskOnTick::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    if (ShouldBroadcastAbilityTaskDelegates())
    {
        OnAbilityTaskTick.Broadcast(DeltaTime);
    }
    else
    {
        EndTask();
    }
}