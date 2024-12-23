// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"


class UWarriorWidgetBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WARRIOR_API UEnemyUIComponent : public UPawnUIComponent
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    void RegisterEnemyDrawnWidget(UWarriorWidgetBase* InWidgetToRegister);

    UFUNCTION(BlueprintCallable)
    void RemoveEnemyDrawnWidgetsIfAny();

private:
    
    UPROPERTY(Transient)
    TArray<TObjectPtr<UWarriorWidgetBase>> EnemyDrawnWidgets;
};