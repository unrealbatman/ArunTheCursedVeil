// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/ArunGameState.h"
#include "Engine/Engine.h"

AArunGameState::AArunGameState()
{
	CurrentChapter = EStoryChapter::Prologue;
	CurrentRegion = ERegion::Dhanpur;

	WorldDarkness = 0.f;

	bBossEncounterActive = false;
}

void AArunGameState::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			TEXT("ArunGameState Initialized"));
	}
}

void AArunGameState::SetCurrentChapter(EStoryChapter NewChapter)
{
	if (CurrentChapter == NewChapter)
	{
		return;
	}

	CurrentChapter = NewChapter;

	OnChapterChanged.Broadcast(CurrentChapter);
}

void AArunGameState::SetCurrentRegion(ERegion NewRegion)
{
	if (CurrentRegion == NewRegion)
	{
		return;
	}

	CurrentRegion = NewRegion;

	OnRegionChanged.Broadcast(CurrentRegion);
}

void AArunGameState::SetCurrentObjective(const FText& NewObjective)
{
	CurrentObjective = NewObjective;

	OnObjectiveChanged.Broadcast(CurrentObjective);
}

void AArunGameState::SetCurrentStoryEvent(FName EventName)
{
	CurrentStoryEvent = EventName;
}

void AArunGameState::SetWorldDarkness(float NewDarkness)
{
	WorldDarkness = FMath::Clamp(NewDarkness, 0.f, 100.f);

	OnWorldDarknessChanged.Broadcast(WorldDarkness);
}

void AArunGameState::AddWorldDarkness(float Amount)
{
	SetWorldDarkness(WorldDarkness + Amount);
}

void AArunGameState::SetWorldFlag(FName FlagName, bool bValue)
{
	WorldFlags.FindOrAdd(FlagName) = bValue;
}

bool AArunGameState::GetWorldFlag(FName FlagName) const
{
	const bool* FoundFlag = WorldFlags.Find(FlagName);

	return FoundFlag ? *FoundFlag : false;
}

void AArunGameState::StartBossEncounter(FName BossName)
{
	if (bBossEncounterActive)
	{
		return;
	}

	bBossEncounterActive = true;
	CurrentBoss = BossName;

	OnBossEncounterStarted.Broadcast(BossName);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Red,
			FString::Printf(TEXT("Boss Encounter Started: %s"), *BossName.ToString()));
	}
}

void AArunGameState::EndBossEncounter()
{
	if (!bBossEncounterActive)
	{
		return;
	}

	const FName EndedBoss = CurrentBoss;

	bBossEncounterActive = false;
	CurrentBoss = NAME_None;

	OnBossEncounterEnded.Broadcast(EndedBoss);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			FString::Printf(TEXT("Boss Encounter Ended: %s"), *EndedBoss.ToString()));
	}
}

EStoryChapter AArunGameState::GetCurrentChapter() const
{
	return CurrentChapter;
}

ERegion AArunGameState::GetCurrentRegion() const
{
	return CurrentRegion;
}

FText AArunGameState::GetCurrentObjective() const
{
	return CurrentObjective;
}

float AArunGameState::GetWorldDarkness() const
{
	return WorldDarkness;
}

bool AArunGameState::IsBossEncounterActive() const
{
	return bBossEncounterActive;
}