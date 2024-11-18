// Valerii Diachenko All Rights Reserved.


#include "Characters/WarriorHeroCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "WarriorAbilitySystemComponent.h"
#include "WarriorDebugHelper.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WarriorGameplayTags.h"
#include "Combat/HeroCombatComponent.h"
#include "Input/WarriorInputComponent.h"
#include "Input/DataAsset_InputConfig.h"
#include "StartUpData/DataAsset_StartUpDataBase.h"
#include "UI/HeroUIComponent.h"

AWarriorHeroCharacter::AWarriorHeroCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 200.f;
    CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    CombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("CombatComponent"));

    HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}

UPawnCombatComponent* AWarriorHeroCharacter::GetPawnCombatComponent() const
{
    return GetCombatComponent();
}

UPawnUIComponent* AWarriorHeroCharacter::GetPawnUIComponent() const
{
    return HeroUIComponent;
}

void AWarriorHeroCharacter::BeginPlay()
{
    Super::BeginPlay();

    Debug::Print("AWarriorHeroCharacter::BeginPlay");
}

void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
    auto* InputSubsystem = IsValid(LocalPlayer) ? LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr;
    if (IsValid(InputSubsystem))
    {
        InputSubsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
    }
    
    UWarriorInputComponent* WarriorInputComponent = CastChecked<UWarriorInputComponent>(PlayerInputComponent);

    WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
    WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

    WarriorInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AWarriorHeroCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (!CharacterStartUpData.IsNull())
    {
        UDataAsset_StartUpDataBase* StartUpData = CharacterStartUpData.LoadSynchronous();
        if (IsValid(StartUpData))
        {
            StartUpData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
        }
    }
}

void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
    const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

    const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

    if (MovementVector.Y != 0.f)
    {
        const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

        AddMovementInput(ForwardDirection, MovementVector.Y);
    }

    if (MovementVector.X != 0.f)
    {
        const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
    const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

    if (LookAxisVector.X != 0.f)
    {
        AddControllerYawInput(LookAxisVector.X);
    }

    if (LookAxisVector.Y != 0.f)
    {
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AWarriorHeroCharacter::Input_AbilityInputPressed(FGameplayTag InputTag)
{
    WarriorAbilitySystemComponent->OnAbilityInputPressed(InputTag);
}

void AWarriorHeroCharacter::Input_AbilityInputReleased(FGameplayTag InputTag)
{
    WarriorAbilitySystemComponent->OnAbilityInputReleased(InputTag);
}
