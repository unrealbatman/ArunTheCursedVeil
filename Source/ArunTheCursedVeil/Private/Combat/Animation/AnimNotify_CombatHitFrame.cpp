#include "Combat/Animation/AnimNotify_CombatHitFrame.h"
#include "Combat/Components/CombatComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

void UAnimNotify_CombatHitFrame::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	if (UCombatComponent* CombatComp = MeshComp->GetOwner()->FindComponentByClass<UCombatComponent>())
	{
		CombatComp->PerformHitFrame(HitSocketName);
	}
}

FString UAnimNotify_CombatHitFrame::GetNotifyName_Implementation() const
{
	return FString("Combat Hit Frame");
}
