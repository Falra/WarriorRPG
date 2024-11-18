// Valerii Diachenko All Rights Reserved.


#include "Widgets/WarriorWidgetBase.h"

#include "Interfaces/PawnUIInterface.h"

void UWarriorWidgetBase::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (const IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
    {
        if (UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent())
        {
            BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
        }
    }
}