// Valerii Diachenko All Rights Reserved.


#include "Gamemodes/WarriorBaseGameMode.h"

AWarriorBaseGameMode::AWarriorBaseGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

int32 AWarriorBaseGameMode::GetPlayerAbilityApplyLevelFromDifficulty() const
{
    int32 AbilityApplyLevel = 1;

    switch (GetCurrentGameDifficulty())
    {
        case EWarriorGameDifficulty::Easy:
            AbilityApplyLevel = 4;
            break;
        case EWarriorGameDifficulty::Normal:
            AbilityApplyLevel = 3;
            break;
        case EWarriorGameDifficulty::Hard:
            AbilityApplyLevel = 2;
            break;
        case EWarriorGameDifficulty::VeryHard:
            AbilityApplyLevel = 1;
            break;
        default:
            break;
    }
    return AbilityApplyLevel;
}

int32 AWarriorBaseGameMode::GetEnemyAbilityApplyLevelFromDifficulty() const
{
    int32 AbilityApplyLevel = 1;

    switch (GetCurrentGameDifficulty())
    {
        case EWarriorGameDifficulty::Easy:
            AbilityApplyLevel = 1;
        break;
        case EWarriorGameDifficulty::Normal:
            AbilityApplyLevel = 2;
        break;
        case EWarriorGameDifficulty::Hard:
            AbilityApplyLevel = 3;
        break;
        case EWarriorGameDifficulty::VeryHard:
            AbilityApplyLevel = 4;
        break;
        default:
            break;
    }
    return AbilityApplyLevel;
}