// Valerii Diachenko All Rights Reserved.


#include "Controllers/WarriorAIController.h"

#include "WarriorDebugHelper.h"
#include "Navigation/CrowdFollowingComponent.h"

AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
    if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
    {
        Debug::Print(TEXT("CrowdFollowingComponent valid"), FColor::Green);
    }
}