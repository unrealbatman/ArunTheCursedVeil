#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CombatHitFrame.generated.h"

/**
 * Placed on an attack montage at the exact frame the weapon should connect.
 * Purely a callback gateway (Phase 5, AnimationBridge role): it never decides
 * damage, it just tells the owner's CombatComponent "now" so combat can trace.
 */
UCLASS()
class ARUNTHECURSEDVEIL_API UAnimNotify_CombatHitFrame : public UAnimNotify
{
	GENERATED_BODY()

protected:
	/** Optional socket to trace from (e.g. a weapon socket). Leave None to use the actor's location. */
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName HitSocketName = NAME_None;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
};
