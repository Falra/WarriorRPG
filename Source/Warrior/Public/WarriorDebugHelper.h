// Valerii Diachenko All Rights Reserved.

#pragma once

namespace Debug
{
    static void Print(const FString &Message, const FColor &Color = FColor::MakeRandomColor(), int32 InKey = -1)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(InKey, 5.f, Color, Message);
            UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
        }
    }

    static void Print(const FString& FloatTitle, float FloatValueToPrint, int32 InKey = -1, const FColor& Color = FColor::MakeRandomColor())
    {
        if (GEngine)
        {
            const FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);
            GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, FinalMsg);
            UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
        }
    }
}