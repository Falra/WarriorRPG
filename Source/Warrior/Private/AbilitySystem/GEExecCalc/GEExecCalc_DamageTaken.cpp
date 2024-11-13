// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/WarriorAttributeSet.h"

struct FWarriorDamageCapture
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);

    FWarriorDamageCapture()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false)
        DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false)
    }
};

static const FWarriorDamageCapture& GetWarriorDamageCapture()
{
    static FWarriorDamageCapture WarriorDamageCapture;
    return WarriorDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
    /* slow way of capture
    auto* AttackPowerProperty = FindFieldChecked<FProperty>(UWarriorAttributeSet::StaticClass(),
        GET_MEMBER_NAME_CHECKED(UWarriorAttributeSet, AttackPower));

    FGameplayEffectAttributeCaptureDefinition AttackPowerDefinition(
        AttackPowerProperty,
        EGameplayEffectAttributeCaptureSource::Source,
        false
    );

    RelevantAttributesToCapture.Add(AttackPowerDefinition);
    */

    RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
    RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefensePowerDef);
}