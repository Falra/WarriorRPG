// Valerii Diachenko All Rights Reserved.


#include "WarriorSurvialGameMode.h"

#include "NavigationSystem.h"
#include "Engine/AssetManager.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

void AWarriorSurvialGameMode::BeginPlay()
{
    Super::BeginPlay();

    checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign a valid data table in survial game mode blueprint"));

    SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::WaitSpawnNewWave);

    TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

    PreLoadNextWaveEnemies();
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
            CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
            
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
                PreLoadNextWaveEnemies();
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

void AWarriorSurvialGameMode::RegisterSpawnedEnemies(const TArray<AWarriorEnemyCharacter*>& InEnemiesToRegister)
{
    for (AWarriorEnemyCharacter* SpawnedEnemy : InEnemiesToRegister)
    {
        if (SpawnedEnemy)
        {
            CurrentSpawnedEnemiesCounter++;
            SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
        }
    }
}

bool AWarriorSurvialGameMode::HasFinishedAllWaves() const
{
    return CurrentWaveCount > TotalWavesToSpawn;
}

void AWarriorSurvialGameMode::PreLoadNextWaveEnemies()
{
    if (HasFinishedAllWaves())
    {
        return;
    }

    PreLoadedEnemyClassMap.Empty();
    
    for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
    {
        if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
        {
            continue;
        }

        UAssetManager::GetStreamableManager().RequestAsyncLoad(
            SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
            FStreamableDelegate::CreateLambda(
                [SpawnerInfo, this]()
                {
                    if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
                    {
                        PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
                    }
                }
                )
            );
    }
}

FWarriorEnemyWaveSpawnerTableRow* AWarriorSurvialGameMode::GetCurrentWaveSpawnerTableRow() const
{
    const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));
    auto* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FWarriorEnemyWaveSpawnerTableRow>(RowName, FString());

    checkf(FoundRow, TEXT("Could not find a valid row under the name %s in the data table"), *RowName.ToString());
    return FoundRow;
}


int32 AWarriorSurvialGameMode::TrySpawnWaveEnemies()
{
    if (TargetPointsArray.IsEmpty())
    {
        UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
    }

    checkf(!TargetPointsArray.IsEmpty(), TEXT("No valid target point found in level: %s for spawning enemies"), *GetWorld()->GetName());

    int32 EnemiesSpawnedThisTime = 0;
    FActorSpawnParameters SpawnParam;
    SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
    {
        if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
        {
            continue;
        }
        
        const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);
        UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

        for (int32 i = 0; i < NumToSpawn; i++)
        {
            const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
            const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
            const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();
            FVector RandomLocation;
            UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.f);
            RandomLocation += FVector(0.f, 0.f, 150.f);

            auto* SpawnedEnemy = GetWorld()->SpawnActor<AWarriorEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParam);
            if (SpawnedEnemy)
            {
                SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
                EnemiesSpawnedThisTime++;
                TotalSpawnedEnemiesThisWaveCounter++;
            }

            if (!ShouldKeepSpawnEnemies())
            {
                return EnemiesSpawnedThisTime;
            }
        }
    }
    return EnemiesSpawnedThisTime;

}

bool AWarriorSurvialGameMode::ShouldKeepSpawnEnemies() const
{
    return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void AWarriorSurvialGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
    CurrentSpawnedEnemiesCounter--;

    if (ShouldKeepSpawnEnemies())
    {
        CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
    }

    else if (CurrentSpawnedEnemiesCounter == 0)
    {
        TotalSpawnedEnemiesThisWaveCounter = 0;
        CurrentSpawnedEnemiesCounter = 0;
        SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::WaveCompleted);
    }
}