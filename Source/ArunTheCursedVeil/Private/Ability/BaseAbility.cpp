#include "Ability/BaseAbility.h"

#include "Character/Arun.h"
#include "Combat/Components/CombatComponent.h"

#include "Engine/Engine.h"
#include "TimerManager.h"

UBaseAbility::UBaseAbility()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBaseAbility::BeginPlay()
{
	Super::BeginPlay();

	OwnerArun = Cast<AArun>(GetOwner());

	CombatComp = GetOwner() ? GetOwner()->FindComponentByClass<UCombatComponent>() : nullptr;

	bUnlocked = bStartsUnlocked;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.f,
			FColor::Cyan,
			FString::Printf(
				TEXT("Ability Initialized: %s"),
				*AbilityName.ToString()));
	}
}

bool UBaseAbility::CanActivateAbility() const
{
	if (!bUnlocked)
	{
		return false;
	}

	if (AbilityState == EAbilityState::Cooldown)
	{
		return false;
	}

	if (AbilityState == EAbilityState::Active)
	{
		return false;
	}

	return true;
}

bool UBaseAbility::ActivateAbility()
{
	if (!CanActivateAbility())
	{
		return false;
	}

	// Ask combat for permission first - combat owns state, abilities only request it (spec section 9).
	if (CombatComp && !CombatComp->TryEnterAbilityState())
	{
		return false;
	}

	AbilityState = EAbilityState::Active;

	OnAbilityActivated.Broadcast();

	BP_OnAbilityActivated();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Green,
			FString::Printf(
				TEXT("%s Activated"),
				*AbilityName.ToString()));
	}

	if (AbilityDuration > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AbilityDurationTimerHandle,
			this,
			&UBaseAbility::HandleAbilityDurationFinished,
			AbilityDuration,
			false);
	}
	else
	{
		DeactivateAbility();
	}

	return true;
}

void UBaseAbility::DeactivateAbility()
{
	if (AbilityState != EAbilityState::Active)
	{
		return;
	}

	if (CombatComp)
	{
		CombatComp->ExitAbilityState();
	}

	BP_OnAbilityDeactivated();

	OnAbilityDeactivated.Broadcast();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Yellow,
			FString::Printf(
				TEXT("%s Deactivated"),
				*AbilityName.ToString()));
	}

	StartCooldown();
}

void UBaseAbility::StartCooldown()
{
	AbilityState = EAbilityState::Cooldown;

	OnAbilityCooldownStarted.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&UBaseAbility::HandleCooldownFinished,
		CooldownDuration,
		false);
}

void UBaseAbility::HandleCooldownFinished()
{
	AbilityState = EAbilityState::Ready;

	OnAbilityCooldownFinished.Broadcast();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.f,
			FColor::Blue,
			FString::Printf(
				TEXT("%s Ready"),
				*AbilityName.ToString()));
	}
}

void UBaseAbility::HandleAbilityDurationFinished()
{
	DeactivateAbility();
}

void UBaseAbility::UnlockAbility()
{
	bUnlocked = true;
}

void UBaseAbility::LockAbility()
{
	bUnlocked = false;
}

bool UBaseAbility::IsUnlocked() const
{
	return bUnlocked;
}

bool UBaseAbility::IsOnCooldown() const
{
	return AbilityState == EAbilityState::Cooldown;
}

bool UBaseAbility::IsAbilityActive() const
{
	return AbilityState == EAbilityState::Active;
}

EAbilityState UBaseAbility::GetAbilityState() const
{
	return AbilityState;
}

float UBaseAbility::GetCooldownDuration() const
{
	return CooldownDuration;
}

float UBaseAbility::GetAbilityDuration() const
{
	return AbilityDuration;
}