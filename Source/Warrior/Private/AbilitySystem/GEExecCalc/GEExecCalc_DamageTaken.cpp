// Valerii Diachenko All Rights Reserved.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"

#include "WarriorDebugHelper.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAttributeSet.h"

struct FWarriorDamageCapture
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
    DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
    DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

    FWarriorDamageCapture()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false)
        DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false)
        DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DamageTaken, Target, false)
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
    RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DamageTakenDef);
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
    //Debug::Print(TEXT("AttackPower"), AttackPower);

    float BaseDamage = 0.f;
    BaseDamage = EffectSpec.GetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_BaseDamage, false, BaseDamage);
    //Debug::Print(TEXT("BaseDamage"), BaseDamage);

    float LightAttackCombo = 0.f;
    LightAttackCombo = EffectSpec.GetSetByCallerMagnitude(WarriorGameplayTags::Player_SetByCaller_AttackType_Light, false, LightAttackCombo);
    //Debug::Print(TEXT("LightAttackCombo"), LightAttackCombo);

    float HeavyAttackCombo = 0.f;
    HeavyAttackCombo = EffectSpec.GetSetByCallerMagnitude(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy, false, HeavyAttackCombo);
    //Debug::Print(TEXT("HeavyAttackCombo"), HeavyAttackCombo);
    
    float DefensePower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().DefensePowerDef, EvalParams, DefensePower);
    //Debug::Print(TEXT("DefensePower"), DefensePower);

    if (LightAttackCombo != 0)
    {
        const float DamageIncreasePercent = (LightAttackCombo - 1) * 0.05f + 1.f;
        BaseDamage *= DamageIncreasePercent;
        //Debug::Print(TEXT("ScaledBaseDamageLight"), BaseDamage);
    }

    if (HeavyAttackCombo != 0)
    {
        const float DamageIncreasePercent = HeavyAttackCombo * 0.15f + 1.f;
        BaseDamage *= DamageIncreasePercent;
        //Debug::Print(TEXT("ScaledBaseDamageHeavy"), BaseDamage);
    }

    const float FinalDamage = BaseDamage * AttackPower / DefensePower;
    Debug::Print(TEXT("FinalDamage"), FinalDamage);

    if (FinalDamage > 0.f)
    {
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(
                GetWarriorDamageCapture().DamageTakenProperty,
                EGameplayModOp::Override,
                FinalDamage
            )
        );
    }
}