#include "Combat/Systems/DamageSystem.h"
#include "Component/HealthComponent.h"
#include "GameFramework/Actor.h"

FDamageResult UDamageSystem::ApplyDamage(const FDamageContext& Context) const
{
	FDamageResult Result;
	Result.DamagedActor = Context.Target;

	AActor* Target = Context.Target;
	if (!Target)
	{
		return Result;
	}

	UHealthComponent* TargetHealth = Target->FindComponentByClass<UHealthComponent>();
	if (!TargetHealth || TargetHealth->IsDead())
	{
		return Result;
	}

	// 1. Apply modifiers (DarkImpulse, resistances, buffs) supplied by the caller.
	float FinalDamage = FMath::Max(0.0f, Context.BaseDamage * Context.DamageMultiplier);

	// 2. Blocking reduces damage but never fully negates it here -
	// bIgnoreBlocks on the attack itself is handled by the caller before this point.
	if (Context.bWasBlocked)
	{
		FinalDamage *= 0.5f;
	}

	// 3. Deduct from health.
	TargetHealth->TakeDamage(FinalDamage);

	// 4. Report what happened.
	Result.FinalDamage = FinalDamage;
	Result.bKilledTarget = TargetHealth->IsDead();
	Result.Outcome = Result.bKilledTarget
		? EDamageOutcome::Fatal
		: (Context.bWasBlocked ? EDamageOutcome::Blocked : EDamageOutcome::Applied);

	return Result;
}
