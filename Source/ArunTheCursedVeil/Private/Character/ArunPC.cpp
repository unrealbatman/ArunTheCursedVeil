#include "Character/ArunPC.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Arun.h"

AArunPC::AArunPC()
{
	bShowMouseCursor = false;
}

void AArunPC::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AArunPC::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AArunPC::Move);
		}

		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction,ETriggerEvent::Triggered,this,&AArunPC::Look);
		}

		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction,ETriggerEvent::Started,this,&AArunPC::JumpPressed);
            EnhancedInput->BindAction(JumpAction,ETriggerEvent::Completed,this,&AArunPC::JumpReleased);
		}

		if (SprintAction)
		{
			EnhancedInput->BindAction(SprintAction,ETriggerEvent::Started,this,&AArunPC::SprintPressed);

			EnhancedInput->BindAction(SprintAction,ETriggerEvent::Completed,this,&AArunPC::SprintReleased);
		}

		if (PauseAction)
		{
			EnhancedInput->BindAction(PauseAction,ETriggerEvent::Started,this,&AArunPC::PausePressed);
		}

		if (LockOnAction)
		{
			EnhancedInput->BindAction(LockOnAction,ETriggerEvent::Started,this,&AArunPC::LockOnPressed);
		}

		if (UnderworldModeAction)
		{
			EnhancedInput->BindAction(UnderworldModeAction,ETriggerEvent::Started,this,&AArunPC::ActivateUnderworldMode);
		}

		if (LightAttackAction)
		{
			EnhancedInput->BindAction(LightAttackAction,ETriggerEvent::Started,this,&AArunPC::LightAttackPressed);
		}

		if (HeavyAttackAction)
		{
			EnhancedInput->BindAction(HeavyAttackAction,ETriggerEvent::Started,this,&AArunPC::HeavyAttackPressed);
		}
	}
}


/* 
============================= Movement Started ====================================
*/
void AArunPC::Move(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();

	if (!ControlledPawn)
	{
		return;
	}

	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection =
		FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection =
		FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	ControlledPawn->AddMovementInput(
		ForwardDirection,
		MovementVector.Y);

	ControlledPawn->AddMovementInput(
		RightDirection,
		MovementVector.X);
}

void AArunPC::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	AddYawInput(LookAxis.X);
	AddPitchInput(LookAxis.Y);
}

void AArunPC::JumpPressed()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->Jump();
	}
}

void AArunPC::JumpReleased()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->StopJumping();
	}
}

void AArunPC::SprintPressed()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = 900.f;
	}
}

void AArunPC::SprintReleased()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

/*
============================= Movement Ended ====================================
*/

void AArunPC::PausePressed()
{
	SetPause(!IsPaused());
}

void AArunPC::LockOnPressed()
{
	bLockOnActive = !bLockOnActive;
}

void AArunPC::ActivateUnderworldMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Underworld Mode Activated"));
}

void AArunPC::LightAttackPressed()
{if (GEngine)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
		TEXT("Arun Not UnderWorld"));
}
	if (AArun* ControlledArun = Cast<AArun>(GetPawn()))
	{
		ControlledArun->PerformLightAttack();
	}
}

void AArunPC::HeavyAttackPressed()
{
	if (AArun* ControlledArun = Cast<AArun>(GetPawn()))
	{
		ControlledArun->PerformHeavyAttack();
	}
}
