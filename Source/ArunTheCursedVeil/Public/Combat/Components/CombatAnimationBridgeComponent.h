#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatAnimationBridgeComponent.generated.h"

class UCombatComponent;

/**
 * UCombatAnimationBridgeComponent - Phase 5 callback gateway.
 * Listens to CombatComponent events and plays/stops the active attack's montage.
 * Purely reactive: it never calls back into combat logic or changes combat state.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARUNTHECURSEDVEIL_API UCombatAnimationBridgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatAnimationBridgeComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void HandleAttackStarted();

	UFUNCTION()
	void HandleAttackEnded();

	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	float MontageBlendOutTime = 0.25f;
};
