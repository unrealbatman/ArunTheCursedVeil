#include "Combat/Components/CombatAnimationBridgeComponent.h"
#include "Combat/Components/CombatComponent.h"
#include "Combat/Data/AttackData.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

UCombatAnimationBridgeComponent::UCombatAnimationBridgeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatAnimationBridgeComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		CombatComp = Owner->FindComponentByClass<UCombatComponent>();
	}

	if (CombatComp)
	{
		CombatComp->OnAttackStarted.AddDynamic(this, &UCombatAnimationBridgeComponent::HandleAttackStarted);
		CombatComp->OnAttackEnded.AddDynamic(this, &UCombatAnimationBridgeComponent::HandleAttackEnded);
	}
}

void UCombatAnimationBridgeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CombatComp)
	{
		CombatComp->OnAttackStarted.RemoveDynamic(this, &UCombatAnimationBridgeComponent::HandleAttackStarted);
		CombatComp->OnAttackEnded.RemoveDynamic(this, &UCombatAnimationBridgeComponent::HandleAttackEnded);
	}

	Super::EndPlay(EndPlayReason);
}

void UCombatAnimationBridgeComponent::HandleAttackStarted()
{
	if (!CombatComp)
	{
		return;
	}

	const UAttackData* AttackData = CombatComp->GetActiveAttackData();
	if (!AttackData || !AttackData->AnimMontage)
	{
		return;
	}

	const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* Mesh = OwnerCharacter ? OwnerCharacter->GetMesh() : nullptr;
	if (!Mesh)
	{
		return;
	}

	if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
	{
		AnimInstance->Montage_Play(AttackData->AnimMontage);
	}
}

void UCombatAnimationBridgeComponent::HandleAttackEnded()
{
	const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* Mesh = OwnerCharacter ? OwnerCharacter->GetMesh() : nullptr;
	if (!Mesh)
	{
		return;
	}

	if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
	{
		const UAttackData* AttackData = CombatComp ? CombatComp->GetActiveAttackData() : nullptr;
		AnimInstance->Montage_Stop(MontageBlendOutTime, AttackData ? AttackData->AnimMontage : nullptr);
	}
}
