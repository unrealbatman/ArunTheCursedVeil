#include "Component/HealthComponent.h"
#include "Engine/Engine.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Green,
			TEXT("Health Component Initialized"));
	}
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
	if (bIsDead)
	{
		return;
	}

	if (DamageAmount <= 0.f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(
		CurrentHealth - DamageAmount,
		0.f,
		MaxHealth);

	OnHealthChanged.Broadcast(
		CurrentHealth,
		MaxHealth);

	if (CurrentHealth <= 0.f)
	{
		Kill();
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	if (bIsDead)
	{
		return;
	}

	if (HealAmount <= 0.f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(
		CurrentHealth + HealAmount,
		0.f,
		MaxHealth);

	OnHealthChanged.Broadcast(
		CurrentHealth,
		MaxHealth);
}

void UHealthComponent::FullHeal()
{
	if (bIsDead)
	{
		return;
	}

	CurrentHealth = MaxHealth;

	OnHealthChanged.Broadcast(
		CurrentHealth,
		MaxHealth);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Green,
			TEXT("Full Heal"));
	}
}

void UHealthComponent::Kill()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	CurrentHealth = 0.f;

	OnHealthChanged.Broadcast(
		CurrentHealth,
		MaxHealth);

	OnDeath.Broadcast();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Red,
			TEXT("Actor Died"));
	}
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetHealthPercent() const
{
	if (MaxHealth <= 0.f)
	{
		return 0.f;
	}

	return CurrentHealth / MaxHealth;
}

bool UHealthComponent::IsDead() const
{
	return bIsDead;
}