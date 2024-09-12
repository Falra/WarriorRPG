// Valerii Diachenko All Rights Reserved.

#pragma once

namespace Debug
{
    static void Print(const FString &Message, const FColor &Color = FColor::MakeRandomColor(), int32 InKey = -1)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(InKey, 5.0f, Color, Message);
            UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
        }
    }
}