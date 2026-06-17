// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ArunGameMode.h"

#include "Core/ArunGameInstance.h"
#include "GameState/ArunGameState.h"

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

AArunGameMode::AArunGameMode()
{
	bGameStarted = false;
	CurrentCheckpoint = NAME_None;
}

void AArunGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitializeGame();
}

void AArunGameMode::InitializeGame()
{
	CachedGameInstance = Cast<UArunGameInstance>(GetGameInstance());
	CachedGameState = GetGameState<AArunGameState>();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			TEXT("ArunGameMode Initialized"));
	}
}

void AArunGameMode::StartNewGame()
{
	bGameStarted = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Cyan,
			TEXT("New Game Started"));
	}
}

void AArunGameMode::AdvanceChapter(EStoryChapter NewChapter)
{
	if (!CachedGameState)
	{
		return;
	}

	CachedGameState->SetCurrentChapter(NewChapter);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			TEXT("Chapter Advanced"));
	}
}

void AArunGameMode::TriggerStoryEvent(FName EventName)
{
	if (!CachedGameState)
	{
		return;
	}

	CachedGameState->SetCurrentStoryEvent(EventName);

	OnStoryEventTriggered.Broadcast(EventName);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Magenta,
			FString::Printf(TEXT("Story Event: %s"), *EventName.ToString()));
	}
}

void AArunGameMode::SetObjective(const FText& NewObjective)
{
	if (!CachedGameState)
	{
		return;
	}

	CachedGameState->SetCurrentObjective(NewObjective);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Blue,
			NewObjective.ToString());
	}
}

void AArunGameMode::ClearObjective()
{
	if (!CachedGameState)
	{
		return;
	}

	CachedGameState->SetCurrentObjective(FText::GetEmpty());

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Blue,
			TEXT("Objective Cleared"));
	}
}

void AArunGameMode::SetCheckpoint(FName CheckpointID)
{
	CurrentCheckpoint = CheckpointID;

	OnCheckpointUpdated.Broadcast(CheckpointID);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Orange,
			FString::Printf(TEXT("Checkpoint: %s"), *CheckpointID.ToString()));
	}
}

FName AArunGameMode::GetCurrentCheckpoint() const
{
	return CurrentCheckpoint;
}

void AArunGameMode::HandlePlayerDeath()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Red,
			TEXT("Player Died"));
	}

	RespawnPlayer();
}

void AArunGameMode::RespawnPlayer()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			TEXT("Respawning Player"));
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	if (!PC)
	{
		return;
	}

	RestartPlayer(PC);
}

void AArunGameMode::StartBossEncounter(FName BossName)
{
	if (!CachedGameState)
	{
		return;
	}

	CachedGameState->StartBossEncounter(BossName);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Red,
			FString::Printf(TEXT("Boss Started: %s"), *BossName.ToString()));
	}
}

void AArunGameMode::EndBossEncounter()
{
	if (!CachedGameState)
	{
		return;
	}

	CachedGameState->EndBossEncounter();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			TEXT("Boss Encounter Ended"));
	}
}

bool AArunGameMode::IsGameStarted() const
{
	return bGameStarted;
}

UArunGameInstance* AArunGameMode::GetArunGameInstance() const
{
	return CachedGameInstance;
}

AArunGameState* AArunGameMode::GetArunGameState() const
{
	return CachedGameState;
}