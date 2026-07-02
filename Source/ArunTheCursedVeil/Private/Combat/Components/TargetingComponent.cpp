#include "Combat/Components/TargetingComponent.h"
#include "Component/HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "CollisionShape.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // On-demand only, no polling.
}

bool UTargetingComponent::TraceForTargets(const FVector& Origin, float Radius, AActor* IgnoreActor, TArray<FHitResult>& OutHits) const
{
	OutHits.Reset();

	const UWorld* World = GetWorld();
	if (!World || Radius <= 0.0f)
	{
		return false;
	}

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(CombatTargetingTrace), false);
	if (IgnoreActor)
	{
		QueryParams.AddIgnoredActor(IgnoreActor);
	}

	// A zero-length sweep with a sphere shape behaves like an overlap check,
	// which is what we want for "who is near this point right now".
	return World->SweepMultiByChannel(
		OutHits,
		Origin,
		Origin,
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(Radius),
		QueryParams);
}

bool UTargetingComponent::IsValidTarget(AActor* Candidate) const
{
	if (!Candidate || Candidate == GetOwner())
	{
		return false;
	}

	const UHealthComponent* CandidateHealth = Candidate->FindComponentByClass<UHealthComponent>();
	if (!CandidateHealth)
	{
		return false;
	}

	return !CandidateHealth->IsDead();
}
