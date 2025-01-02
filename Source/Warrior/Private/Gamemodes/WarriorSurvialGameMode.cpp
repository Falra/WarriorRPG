// Valerii Diachenko All Rights Reserved.


#include "WarriorSurvialGameMode.h"

void AWarriorSurvialGameMode::BeginPlay()
{
    Super::BeginPlay();

    checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign a valid data table in survial game mode blueprint"));

    SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::WaitSpawnNewWave);

    TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
}

void AWarriorSurvialGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentSurvialGameModeState == EWarriorSurvialGameModeState::WaitSpawnNewWave)
    {
        TimePassedSinceStart += DeltaTime;
        if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
        {
            TimePassedSinceStart = 0.f;
            SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::SpawningNewWave);
        }
    }
    
    if (CurrentSurvialGameModeState == EWarriorSurvialGameModeState::SpawningNewWave)
    {
        TimePassedSinceStart += DeltaTime;
        if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
        {
            // TODO: Handle spawn new enemies
            TimePassedSinceStart = 0.f;
            SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::InProgress);
        }
    }
    
    if (CurrentSurvialGameModeState == EWarriorSurvialGameModeState::WaveCompleted)
    {
        TimePassedSinceStart += DeltaTime;
        if (TimePassedSinceStart >= WaveCompletedWaitTime)
        {
            TimePassedSinceStart = 0.f;
            CurrentWaveCount++;
            if (HasFinishedAllWaves())
            {
                SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::AllWavesDone);
            }
            else
            {
                SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::WaitSpawnNewWave);
            }
        }
    }
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

bool AWarriorSurvialGameMode::HasFinishedAllWaves() const
{
    return CurrentWaveCount > TotalWavesToSpawn;
}