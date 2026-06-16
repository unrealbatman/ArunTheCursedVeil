#pragma once

#include "CoreMinimal.h"
#include "ArunTypes.generated.h"

/*
|--------------------------------------------------------------------------
| Story Chapters
|--------------------------------------------------------------------------
|
| Controls overall narrative progression.
|
*/

UENUM(BlueprintType)
enum class EStoryChapter : uint8
{
	Prologue				UMETA(DisplayName = "Prologue"),

	DhanpurBurning			UMETA(DisplayName = "Dhanpur Burning"),

	JourneyBegins			UMETA(DisplayName = "Journey Begins"),

	VasishtReveal			UMETA(DisplayName = "Vasisht Reveal"),

	AstraAwakening			UMETA(DisplayName = "Astra Awakening"),

	DemonWar				UMETA(DisplayName = "Demon War"),

	FinalAscension			UMETA(DisplayName = "Final Ascension")
};

/*
|--------------------------------------------------------------------------
| Regions
|--------------------------------------------------------------------------
|
| Represents the player's current world location.
|
*/

UENUM(BlueprintType)
enum class ERegion : uint8
{
	Dhanpur					UMETA(DisplayName = "Dhanpur"),

	AndhkaarForest			UMETA(DisplayName = "Andhkaar Forest"),

	AndhkaarGufa			UMETA(DisplayName = "Andhkaar Gufa"),

	AncientTemple			UMETA(DisplayName = "Ancient Temple"),

	DevlokGate				UMETA(DisplayName = "Devlok Gate")
};

/*
|--------------------------------------------------------------------------
| Corruption Stage
|--------------------------------------------------------------------------
|
| Determines Arun's corruption progression.
|
*/

UENUM(BlueprintType)
enum class ECorruptionStage : uint8
{
	Pure					UMETA(DisplayName = "Pure"),

	Tainted					UMETA(DisplayName = "Tainted"),

	Corrupted				UMETA(DisplayName = "Corrupted"),

	Consumed				UMETA(DisplayName = "Consumed")
};

/*
|--------------------------------------------------------------------------
| Boss Encounter State
|--------------------------------------------------------------------------
|
| Used by GameMode / GameState.
|
*/

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Inactive				UMETA(DisplayName = "Inactive"),

	Preparing				UMETA(DisplayName = "Preparing"),

	Active					UMETA(DisplayName = "Active"),

	Completed				UMETA(DisplayName = "Completed")
};

/*
|--------------------------------------------------------------------------
| Objective State
|--------------------------------------------------------------------------
|
| Future quest / objective system support.
|
*/

UENUM(BlueprintType)
enum class EObjectiveState : uint8
{
	Inactive				UMETA(DisplayName = "Inactive"),

	Active					UMETA(DisplayName = "Active"),

	Completed				UMETA(DisplayName = "Completed"),

	Failed					UMETA(DisplayName = "Failed")
};

/*
|--------------------------------------------------------------------------
| Player Life State
|--------------------------------------------------------------------------
|
| Used by Character, GameMode and UI.
|
*/

UENUM(BlueprintType)
enum class EPlayerLifeState : uint8
{
	Alive					UMETA(DisplayName = "Alive"),

	Dead					UMETA(DisplayName = "Dead")
};

/*
|--------------------------------------------------------------------------
| Underworld State
|--------------------------------------------------------------------------
|
| Core mechanic of Arun.
|
*/

UENUM(BlueprintType)
enum class EUnderworldState : uint8
{
	Locked					UMETA(DisplayName = "Locked"),

	Available				UMETA(DisplayName = "Available"),

	Active					UMETA(DisplayName = "Active")
};