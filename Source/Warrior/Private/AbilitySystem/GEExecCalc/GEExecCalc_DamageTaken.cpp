// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"

#include "WarriorGameplayTags.h"
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

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

    // EffectSpec.GetContext().GetSourceObject()
    // EffectSpec.GetContext().GetAbility()
    // EffectSpec.GetContext().GetInstigator()
    // EffectSpec.GetContext().GetEffectCauser()
    
    FAggregatorEvaluateParameters EvalParams;
    EvalParams.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
    EvalParams.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

    float AttackPower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().AttackPowerDef, EvalParams, AttackPower);

    float BaseDamage = 0.f;
    BaseDamage = EffectSpec.GetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_BaseDamage, false, BaseDamage);

    float LightAttackCombo = 0.f;
    LightAttackCombo = EffectSpec.GetSetByCallerMagnitude(WarriorGameplayTags::Player_SetByCaller_AttackType_Light, false, LightAttackCombo);

    float HeavyAttackCombo = 0.f;
    HeavyAttackCombo = EffectSpec.GetSetByCallerMagnitude(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy, false, HeavyAttackCombo);
    
    float DefensePower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().DefensePowerDef, EvalParams, DefensePower);
}