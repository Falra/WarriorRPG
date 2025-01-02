// Valerii Diachenko All Rights Reserved.


#include "WarriorSurvialGameMode.h"

void AWarriorSurvialGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void AWarriorSurvialGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWarriorSurvialGameMode::SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState InState)
{
    if (CurrentSurvialGameModeState == InState)
    {
        return;
    }
    
    CurrentSurvialGameModeState = InState;
    
    OnSurvialGameModeStateChanged.Broadcast(CurrentSurvialGameModeState);
}