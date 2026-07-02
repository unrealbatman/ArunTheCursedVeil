#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatFXComponent.generated.h"

class UCombatComponent;

/**
 * UCombatFXComponent - Phase 8 FX gateway.
 * Subscribes to CombatComponent events and exposes BlueprintImplementableEvents so
 * designers can spawn Niagara/sound FX in Blueprint. Combat never spawns FX itself,
 * and this component never reads or changes combat state.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARUNTHECURSEDVEIL_API UCombatFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatFXComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void HandleHitConfirmed(const FHitResult& HitResult);

	UFUNCTION()
	void HandleDamageApplied(float Damage);

	UFUNCTION()
	void HandleBlocked(AActor* Blocker);

	UFUNCTION()
	void HandleDeath();

	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComp;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat|FX")
	void BP_PlayHitFX(const FHitResult& HitResult);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat|FX")
	void BP_PlayDamageFX(float Damage);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat|FX")
	void BP_PlayBlockFX(AActor* Blocker);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat|FX")
	void BP_PlayDeathFX();
};
