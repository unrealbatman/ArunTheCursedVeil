#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatData.generated.h"

UCLASS(BlueprintType)
class ARUNTHECURSEDVEIL_API UCombatData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Timing")
	float MinFrameTime = 0.016f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Timing")
	float MaxCombatRecovery = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Damage")
	float DefaultBlockDamageReduction = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Targeting")
	int32 MaxSimultaneousTargets = 5;
};
