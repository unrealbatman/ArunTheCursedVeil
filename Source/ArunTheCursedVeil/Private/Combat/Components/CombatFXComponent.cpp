#include "Combat/Components/CombatFXComponent.h"
#include "Combat/Components/CombatComponent.h"
#include "GameFramework/Actor.h"

UCombatFXComponent::UCombatFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatFXComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		CombatComp = Owner->FindComponentByClass<UCombatComponent>();
	}

	if (CombatComp)
	{
		CombatComp->OnHitConfirmed.AddDynamic(this, &UCombatFXComponent::HandleHitConfirmed);
		CombatComp->OnDamageApplied.AddDynamic(this, &UCombatFXComponent::HandleDamageApplied);
		CombatComp->OnBlocked.AddDynamic(this, &UCombatFXComponent::HandleBlocked);
		CombatComp->OnDeath.AddDynamic(this, &UCombatFXComponent::HandleDeath);
	}
}

void UCombatFXComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CombatComp)
	{
		CombatComp->OnHitConfirmed.RemoveDynamic(this, &UCombatFXComponent::HandleHitConfirmed);
		CombatComp->OnDamageApplied.RemoveDynamic(this, &UCombatFXComponent::HandleDamageApplied);
		CombatComp->OnBlocked.RemoveDynamic(this, &UCombatFXComponent::HandleBlocked);
		CombatComp->OnDeath.RemoveDynamic(this, &UCombatFXComponent::HandleDeath);
	}

	Super::EndPlay(EndPlayReason);
}

void UCombatFXComponent::HandleHitConfirmed(const FHitResult& HitResult)
{
	BP_PlayHitFX(HitResult);
}

void UCombatFXComponent::HandleDamageApplied(float Damage)
{
	BP_PlayDamageFX(Damage);
}

void UCombatFXComponent::HandleBlocked(AActor* Blocker)
{
	BP_PlayBlockFX(Blocker);
}

void UCombatFXComponent::HandleDeath()
{
	BP_PlayDeathFX();
}
