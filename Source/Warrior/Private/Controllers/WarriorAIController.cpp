// Valerii Diachenko All Rights Reserved.


#include "Controllers/WarriorAIController.h"

#include "WarriorDebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
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

void AWarriorAIController::BeginPlay()
{
    Super::BeginPlay();

    if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
    {
        CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

        switch (DetourCrowdAvoidanceQuality)
        {
            case 1:
                CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);
                break;
            case 2:
                CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
                break;
            case 3:
                CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
                break;
            case 4:
                CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
                break;
            default:
                break;
        }
        
        CrowdComp->SetAvoidanceGroup(1);
        CrowdComp->SetGroupsToAvoid(1);
        CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
    }
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Stimulus.WasSuccessfullySensed() && IsValid(Actor))
    {
        if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
        {
            BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
        }
    }
}