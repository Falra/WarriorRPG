// Valerii Diachenko All Rights Reserved.


#include "Characters/WarriorEnemyCharacter.h"

#include "WarriorDebugHelper.h"
#include "Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StartUpData/DataAsset_StartUpDataBase.h"

AWarriorEnemyCharacter::AWarriorEnemyCharacter()
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 300.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

    CombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("CombatComponent"));
}

UPawnCombatComponent* AWarriorEnemyCharacter::GetPawnCombatComponent() const
{
    return GetCombatComponent();
}

void AWarriorEnemyCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    InitEnemyStartUpData();
}

void AWarriorEnemyCharacter::InitEnemyStartUpData()
{
    if (CharacterStartUpData.IsNull())
    {
        return;
    }

    UAssetManager::GetStreamableManager().RequestAsyncLoad(CharacterStartUpData.ToSoftObjectPath(),
        FStreamableDelegate::CreateWeakLambda(this, [this]
        {
            UDataAsset_StartUpDataBase* StartUpData = CharacterStartUpData.Get();
            if (IsValid(StartUpData))
            {
                StartUpData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
                Debug::Print(TEXT("Enemy Start Up Data loaded"), FColor::Green);
            }
        }));
}