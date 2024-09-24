// Valerii Diachenko All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WARRIOR_API UPawnExtensionComponentBase : public UActorComponent
{
    GENERATED_BODY()

protected:

    template<class T>
    T* GetOwningPawn() const
    {
        //static_assert(TIsDerivedFrom<T, APawn>::IsDerived, "'T' template parameter to GetPawn must be derived from APawn");
        static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' template parameter to GetPawn must be derived from APawn");
        return CastChecked<T>(GetOwner());
    }

    APawn* GetOwningPawn() const
    {
        return GetOwningPawn<APawn>();
    }

    template<class T>
    T* GetOwningController() const
    {
        static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' template parameter to GetOwningController must be derived from AController");
		return GetOwningPawn()->GetController<T>();
    }
};