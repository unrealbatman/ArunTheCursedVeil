#include "Component/DarkImpulseComponent.h"
#include "Engine/Engine.h"

UDarkImpulseComponent::UDarkImpulseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDarkImpulseComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateCorruptionStage();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Purple,
			TEXT("Dark Impulse Component Initialized"));
	}
}

void UDarkImpulseComponent::AddDarkImpulse(float Amount)
{
	if (Amount <= 0.f)
	{
		return;
	}

	DarkImpulse = FMath::Clamp(
		DarkImpulse + Amount,
		0.f,
		100.f);

	Humanity = FMath::Clamp(
		Humanity - (Amount * 0.5f),
		0.f,
		100.f);

	BroadcastDarkImpulseUpdate();
	BroadcastHumanityUpdate();

	UpdateCorruptionStage();
}

void UDarkImpulseComponent::ReduceDarkImpulse(float Amount)
{
	if (Amount <= 0.f)
	{
		return;
	}

	DarkImpulse = FMath::Clamp(
		DarkImpulse - Amount,
		0.f,
		100.f);

	BroadcastDarkImpulseUpdate();

	UpdateCorruptionStage();
}

void UDarkImpulseComponent::AddHumanity(float Amount)
{
	if (Amount <= 0.f)
	{
		return;
	}

	Humanity = FMath::Clamp(
		Humanity + Amount,
		0.f,
		100.f);

	BroadcastHumanityUpdate();
}

void UDarkImpulseComponent::ReduceHumanity(float Amount)
{
	if (Amount <= 0.f)
	{
		return;
	}

	Humanity = FMath::Clamp(
		Humanity - Amount,
		0.f,
		100.f);

	BroadcastHumanityUpdate();
}

void UDarkImpulseComponent::ActivateUnderworldMode()
{
	if (bUnderworldModeActive)
	{
		return;
	}

	if (DarkImpulse < 50.f)
	{
		return;
	}

	bUnderworldModeActive = true;

	OnUnderworldActivated.Broadcast();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Red,
			TEXT("UNDERWORLD MODE ACTIVATED"));
	}
}

void UDarkImpulseComponent::DeactivateUnderworldMode()
{
	if (!bUnderworldModeActive)
	{
		return;
	}

	bUnderworldModeActive = false;

	OnUnderworldDeactivated.Broadcast();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			TEXT("UNDERWORLD MODE DEACTIVATED"));
	}
}

void UDarkImpulseComponent::UpdateCorruptionStage()
{
	ECorruptionStage NewStage = ECorruptionStage::Pure;

	if (DarkImpulse >= 75.f)
	{
		NewStage = ECorruptionStage::Consumed;
	}
	else if (DarkImpulse >= 50.f)
	{
		NewStage = ECorruptionStage::Corrupted;
	}
	else if (DarkImpulse >= 25.f)
	{
		NewStage = ECorruptionStage::Tainted;
	}

	if (CorruptionStage != NewStage)
	{
		CorruptionStage = NewStage;

		OnCorruptionStageChanged.Broadcast(
			CorruptionStage);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Orange,
				TEXT("Corruption Stage Updated"));
		}
	}
}

void UDarkImpulseComponent::BroadcastDarkImpulseUpdate()
{
	OnDarkImpulseChanged.Broadcast(
		DarkImpulse);
}

void UDarkImpulseComponent::BroadcastHumanityUpdate()
{
	OnHumanityChanged.Broadcast(
		Humanity);
}

float UDarkImpulseComponent::GetDarkImpulse() const
{
	return DarkImpulse;
}

float UDarkImpulseComponent::GetHumanity() const
{
	return Humanity;
}

float UDarkImpulseComponent::GetDarkImpulsePercent() const
{
	return DarkImpulse / 100.f;
}

float UDarkImpulseComponent::GetHumanityPercent() const
{
	return Humanity / 100.f;
}

ECorruptionStage UDarkImpulseComponent::GetCorruptionStage() const
{
	return CorruptionStage;
}

bool UDarkImpulseComponent::IsUnderworldModeActive() const
{
	return bUnderworldModeActive;
}