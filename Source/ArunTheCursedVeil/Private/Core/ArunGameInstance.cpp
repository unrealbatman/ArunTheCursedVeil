#include "Core/ArunGameInstance.h"
#include "Engine/Engine.h"

UArunGameInstance::UArunGameInstance()
{
}

void UArunGameInstance::Init()
{
	Super::Init();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			TEXT("ArunGameInstance Initialized"));
	}
}

void UArunGameInstance::Shutdown()
{
	Super::Shutdown();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Red,
			TEXT("ArunGameInstance Shutdown"));
	}
}

void UArunGameInstance::SetStoryFlag(FName FlagName, bool bValue)
{
	StoryFlags.FindOrAdd(FlagName) = bValue;
}

bool UArunGameInstance::GetStoryFlag(FName FlagName) const
{
	const bool* FoundFlag = StoryFlags.Find(FlagName);

	if (FoundFlag)
	{
		return *FoundFlag;
	}

	return false;
}

void UArunGameInstance::UnlockAstra(FName AstraName)
{
	if (!UnlockedAstras.Contains(AstraName))
	{
		UnlockedAstras.Add(AstraName);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Yellow,
				FString::Printf(TEXT("Unlocked Astra: %s"), *AstraName.ToString()));
		}
	}
}

void UArunGameInstance::RegisterBossDefeat(FName BossName)
{
	if (!DefeatedBosses.Contains(BossName))
	{
		DefeatedBosses.Add(BossName);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Cyan,
				FString::Printf(TEXT("Boss Defeated: %s"), *BossName.ToString()));
		}
	}
}