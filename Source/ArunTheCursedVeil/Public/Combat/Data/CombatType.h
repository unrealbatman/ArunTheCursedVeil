// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatType.generated.h"

// Forward declaration only - structs below merely hold UAttackData* pointers,
// and AttackData.h itself needs the enums defined in this file (EAttackType).
class UAttackData;


/**
 * ECombatState - Defines all possible combat states for the combat framework
 * Phase 1: Core state machine for basic combat actions and recovery
 */
UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Idle UMETA(DisplayName = "Idle"),           // Ready to act, no recovery time
	Attacking UMETA(DisplayName = "Attacking"), // Attack active, no interruption
	Recover UMETA(DisplayName = "Recover"),     // Post-attack recovery window
	Hit UMETA(DisplayName = "Hit"),             // Receiving damage, staggered
	Blocking UMETA(DisplayName = "Blocking"),   // Active block, reduced damage
	Dead UMETA(DisplayName = "Dead"),           // Permanently unable to act
	Ability UMETA(DisplayName = "Ability"),     // Ability executing
	Underworld UMETA(DisplayName = "Underworld") // Dark impulse state active
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Light UMETA(DisplayName = "Light"),
	Heavy UMETA(DisplayName = "Heavy"),
	Charged UMETA(DisplayName = "Charged"),
	Combo UMETA(DisplayName = "Combo"),
	Ability UMETA(DisplayName = "Ability")
};

UENUM(BlueprintType)
enum class EDamageOutcome : uint8
{
	None UMETA(DisplayName = "None"),
	Applied UMETA(DisplayName = "Applied"),
	Blocked UMETA(DisplayName = "Blocked"),
	Absorbed UMETA(DisplayName = "Absorbed"),
	Fatal UMETA(DisplayName = "Fatal")
};

USTRUCT(BlueprintType)
struct ARUNTHECURSEDVEIL_API FCombatAttackRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UAttackData> AttackData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAttackType AttackType = EAttackType::Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector InputDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bWantsComboContinuation = false;
};

USTRUCT(BlueprintType)
struct ARUNTHECURSEDVEIL_API FHitContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> Attacker = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> Defender = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FVector HitLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FVector HitNormal = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAttackData> AttackSource = nullptr;
};

USTRUCT(BlueprintType)
struct ARUNTHECURSEDVEIL_API FBlockContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> Blocker = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> Attacker = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FVector BlockLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float IncomingDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float DamageReduction = 0.5f;
};

USTRUCT(BlueprintType)
struct ARUNTHECURSEDVEIL_API FDamageContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> Source = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> Target = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float DamageMultiplier = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bWasBlocked = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAttackData> AttackSource = nullptr;
};

USTRUCT(BlueprintType)
struct ARUNTHECURSEDVEIL_API FDamageResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	EDamageOutcome Outcome = EDamageOutcome::None;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float FinalDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bKilledTarget = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> DamagedActor = nullptr;
};
