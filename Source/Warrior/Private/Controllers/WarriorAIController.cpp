// Valerii Diachenko All Rights Reserved.


#include "Controllers/WarriorAIController.h"

#include "WarriorDebugHelper.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{

    AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
    AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
    AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
    AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
    AISenseConfig_Sight->SightRadius = 5000.f;
    AISenseConfig_Sight->LoseSightRadius = 0.f;
    AISenseConfig_Sight->PeripheralVisionAngleDegrees = 180.f;

    EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
    EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
    EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
    EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

    SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    const auto* PawnToCheck = Cast<const APawn>(&Other);
    const auto* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController());
    if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
    {
        return ETeamAttitude::Hostile;
    }
    return ETeamAttitude::Friendly;
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Stimulus.WasSuccessfullySensed() && Actor)
    {
        Debug::Print(Actor->GetActorNameOrLabel() + TEXT(" was sensed"), FColor::Green);
    }
}