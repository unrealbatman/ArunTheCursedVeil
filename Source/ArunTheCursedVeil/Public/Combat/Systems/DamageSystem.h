#pragma once

#include "CoreMinimal.h"
#include "Combat/Data/CombatType.h"
#include "DamageSystem.generated.h"

/**
 * UDamageSystem - Execution Engine (Phase 2).
 * Applies damage after a hit has already been confirmed. Never decides
 * whether to attack, whether a hit landed, or how much to deal on its own -
 * it only executes the FDamageContext it is handed and reports what happened.
 */
UCLASS(BlueprintType)
class ARUNTHECURSEDVEIL_API UDamageSystem : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Combat|Damage")
	FDamageResult ApplyDamage(const FDamageContext& Context) const;
};
