#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Enums/ArunTypes.h"
#include "ArunGameInstance.generated.h"


UCLASS()
class ARUNTHECURSEDVEIL_API UArunGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UArunGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

#pragma region Story

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	EStoryChapter CurrentChapter = EStoryChapter::Prologue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	ERegion CurrentRegion = ERegion::Dhanpur;

#pragma endregion

#pragma region Matrashana

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Matrashana")
	int32 PiecesCollected = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Matrashana")
	int32 TotalPieces = 4;

#pragma endregion

#pragma region Karma

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Karma")
	float HumanityValue = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Karma")
	float DarknessValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Karma")
	ECorruptionStage CorruptionStage = ECorruptionStage::Pure;

#pragma endregion

#pragma region Underworld

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Underworld")
	float DarkImpulseLevel = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Underworld")
	bool bUnderworldUnlocked = false;

#pragma endregion

#pragma region Astras

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astras")
	TArray<FName> UnlockedAstras;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astras")
	FName EquippedAstra;

#pragma endregion

#pragma region Bosses

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bosses")
	TArray<FName> DefeatedBosses;

#pragma endregion

#pragma region StoryFlags

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story Flags")
	TMap<FName, bool> StoryFlags;

#pragma endregion

#pragma region SaveData

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	FString CurrentSaveSlot = TEXT("Slot_01");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	FName LastCheckpoint;

#pragma endregion

public:

	UFUNCTION(BlueprintCallable, Category = "Story")
	void SetStoryFlag(FName FlagName, bool bValue);

	UFUNCTION(BlueprintCallable, Category = "Story")
	bool GetStoryFlag(FName FlagName) const;

	UFUNCTION(BlueprintCallable, Category = "Astras")
	void UnlockAstra(FName AstraName);

	UFUNCTION(BlueprintCallable, Category = "Bosses")
	void RegisterBossDefeat(FName BossName);
};