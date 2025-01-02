#pragma once

UENUM()
enum class EWarriorCountDownActionInput : uint8
{
    Start,
    Cancel
};

UENUM()
enum class EWarriorCountDownActionOutput : uint8
{
    Updated,
    Completed,
    Cancelled
};

UENUM(BlueprintType)
enum class EWarriorGameDifficulty : uint8
{
    Easy,
    Normal,
    Hard,
    VeryHard
};