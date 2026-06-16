#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Enums/ArunTypes.h"
#include "ArunGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnChapterChanged,
	EStoryChapter,
	NewChapter);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnRegionChanged,
	ERegion,
	NewRegion);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnObjectiveChanged,
	FText,
	NewObjective);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnWorldDarknessChanged,
	float,
	NewDarkness);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnBossEncounterStarted,
	FName,
	BossName);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnBossEncounterEnded,
	FName,
	BossName);

UCLASS()
class ARUNTHECURSEDVEIL_API AArunGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AArunGameState();

protected:

	virtual void BeginPlay() override;

#pragma region Story

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Story")
	EStoryChapter CurrentChapter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Story")
	ERegion CurrentRegion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Story")
	FText CurrentObjective;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Story")
	FName CurrentStoryEvent;

#pragma endregion

#pragma region World

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	float WorldDarkness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	TMap<FName, bool> WorldFlags;

#pragma endregion

#pragma region Boss

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	bool bBossEncounterActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	FName CurrentBoss;

#pragma endregion

public:

#pragma region Delegates

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnChapterChanged OnChapterChanged;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnRegionChanged OnRegionChanged;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnObjectiveChanged OnObjectiveChanged;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnWorldDarknessChanged OnWorldDarknessChanged;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnBossEncounterStarted OnBossEncounterStarted;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnBossEncounterEnded OnBossEncounterEnded;

#pragma endregion

#pragma region Story Functions

	UFUNCTION(BlueprintCallable, Category = "Story")
	void SetCurrentChapter(EStoryChapter NewChapter);

	UFUNCTION(BlueprintCallable, Category = "Story")
	void SetCurrentRegion(ERegion NewRegion);

	UFUNCTION(BlueprintCallable, Category = "Story")
	void SetCurrentObjective(const FText& NewObjective);

	UFUNCTION(BlueprintCallable, Category = "Story")
	void SetCurrentStoryEvent(FName EventName);

#pragma endregion

#pragma region World Functions

	UFUNCTION(BlueprintCallable, Category = "World")
	void SetWorldDarkness(float NewDarkness);

	UFUNCTION(BlueprintCallable, Category = "World")
	void AddWorldDarkness(float Amount);

	UFUNCTION(BlueprintCallable, Category = "World")
	void SetWorldFlag(FName FlagName, bool bValue);

	UFUNCTION(BlueprintCallable, Category = "World")
	bool GetWorldFlag(FName FlagName) const;

#pragma endregion

#pragma region Boss Functions

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void StartBossEncounter(FName BossName);

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void EndBossEncounter();

#pragma endregion

#pragma region Getters

	UFUNCTION(BlueprintPure)
	EStoryChapter GetCurrentChapter() const;

	UFUNCTION(BlueprintPure)
	ERegion GetCurrentRegion() const;

	UFUNCTION(BlueprintPure)
	FText GetCurrentObjective() const;

	UFUNCTION(BlueprintPure)
	float GetWorldDarkness() const;

	UFUNCTION(BlueprintPure)
	bool IsBossEncounterActive() const;

#pragma endregion
};