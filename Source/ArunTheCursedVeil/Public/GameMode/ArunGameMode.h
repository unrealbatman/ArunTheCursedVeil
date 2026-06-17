#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Enums/ArunTypes.h"
#include "ArunGameMode.generated.h"

class UArunGameInstance;
class AArunGameState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnStoryEventTriggered,
	FName,
	EventName);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCheckpointUpdated,
	FName,
	CheckpointID);

UCLASS()
class ARUNTHECURSEDVEIL_API AArunGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AArunGameMode();

protected:

	virtual void BeginPlay() override;

#pragma region Cached References

	UPROPERTY()
	TObjectPtr<UArunGameInstance> CachedGameInstance;

	UPROPERTY()
	TObjectPtr<AArunGameState> CachedGameState;

#pragma endregion

#pragma region Runtime Data

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	bool bGameStarted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	FName CurrentCheckpoint;

#pragma endregion

public:

#pragma region Delegates

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnStoryEventTriggered OnStoryEventTriggered;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnCheckpointUpdated OnCheckpointUpdated;

#pragma endregion

#pragma region Initialization

	UFUNCTION(BlueprintCallable, Category = "Game")
	void InitializeGame();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void StartNewGame();

#pragma endregion

#pragma region Story

	UFUNCTION(BlueprintCallable, Category = "Story")
	void AdvanceChapter(EStoryChapter NewChapter);

	UFUNCTION(BlueprintCallable, Category = "Story")
	void TriggerStoryEvent(FName EventName);

#pragma endregion

#pragma region Objectives

	UFUNCTION(BlueprintCallable, Category = "Objectives")
	void SetObjective(const FText& NewObjective);

	UFUNCTION(BlueprintCallable, Category = "Objectives")
	void ClearObjective();

#pragma endregion

#pragma region Checkpoints

	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	void SetCheckpoint(FName CheckpointID);

	UFUNCTION(BlueprintPure, Category = "Checkpoint")
	FName GetCurrentCheckpoint() const;

#pragma endregion

#pragma region Death

	UFUNCTION(BlueprintCallable, Category = "Player")
	void HandlePlayerDeath();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void RespawnPlayer();

#pragma endregion

#pragma region Boss

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void StartBossEncounter(FName BossName);

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void EndBossEncounter();

#pragma endregion

#pragma region Getters

	UFUNCTION(BlueprintPure, Category = "Game")
	bool IsGameStarted() const;

	UFUNCTION(BlueprintPure, Category = "Game")
	UArunGameInstance* GetArunGameInstance() const;

	UFUNCTION(BlueprintPure, Category = "Game")
	AArunGameState* GetArunGameState() const;

#pragma endregion
};