#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "TargetingComponent.generated.h"

/**
 * UTargetingComponent - Acquisition & Validation (Phase 4).
 * Finds and validates potential targets. Never moves the character and
 * never decides whether to deal damage - that stays in CombatComponent/DamageSystem.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARUNTHECURSEDVEIL_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTargetingComponent();

	/** Sphere-traces for potential targets around Origin, ignoring IgnoreActor (usually the owner). */
	UFUNCTION(BlueprintCallable, Category = "Combat|Targeting")
	bool TraceForTargets(const FVector& Origin, float Radius, AActor* IgnoreActor, TArray<FHitResult>& OutHits) const;

	/** A target is valid if it exists, isn't the owner, has a HealthComponent, and isn't dead. */
	UFUNCTION(BlueprintPure, Category = "Combat|Targeting")
	bool IsValidTarget(AActor* Candidate) const;

	/** Collision channel used when tracing for attack targets. Defaults to Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Targeting")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Pawn;
};
