// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WarriorAIController.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorAIController : public AAIController
{
    GENERATED_BODY()

public:
    AWarriorAIController(const FObjectInitializer& ObjectInitializer);

protected:
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
    TObjectPtr<UAIPerceptionComponent> EnemyPerceptionComponent;

    UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
    TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;

    UFUNCTION()
    virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};