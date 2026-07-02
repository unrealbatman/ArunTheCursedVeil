#include "Character/Arun.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Combat/Components/CombatComponent.h"
#include "Combat/Data/CombatType.h"

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

	// CombatComponent is added via the character Blueprint (alongside Health/DarkImpulse), not in C++.
	CombatComp = FindComponentByClass<UCombatComponent>();

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

void AArun::PerformLightAttack()
{
	if (!CombatComp)
	{
		return;
	}

	FCombatAttackRequest Request;
	Request.AttackData = LightAttackData;
	CombatComp->Attack(Request);
}

void AArun::PerformHeavyAttack()
{
	if (!CombatComp)
	{
		return;
	}

	FCombatAttackRequest Request;
	Request.AttackData = HeavyAttackData;
	CombatComp->HeavyAttack(Request);
}