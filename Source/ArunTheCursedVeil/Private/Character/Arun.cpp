#include "Character/Arun.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AArun::AArun()
{
	PrimaryActorTick.bCanEverTick = false;

	// ---------------------------------
	// Character Rotation
	// ---------------------------------

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ---------------------------------
	// Character Movement
	// ---------------------------------

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	GetCharacterMovement()->JumpZVelocity = 700.f;

	GetCharacterMovement()->AirControl = 0.35f;

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// ---------------------------------
	// Camera Boom
	// ---------------------------------

	CameraBoom =
		CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 350.f;

	CameraBoom->bUsePawnControlRotation = true;

	// ---------------------------------
	// Follow Camera
	// ---------------------------------

	FollowCamera =
		CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);

	FollowCamera->bUsePawnControlRotation = false;
}

void AArun::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			TEXT("Arun Initialized"));
	}
}

void AArun::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			TEXT("Arun Died"));
	}
}

void AArun::ActivateUnderworldMode()
{
	if (bUnderworldMode)
	{
		return;
	}

	bUnderworldMode = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			TEXT("Arun UnderWorld"));
	}
}

void AArun::DeactivateUnderworldMode()
{
	if (!bUnderworldMode)
	{
		return;
	}

	bUnderworldMode = false;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			TEXT("Arun Not UnderWorld"));
	}
}