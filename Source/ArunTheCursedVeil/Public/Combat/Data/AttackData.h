#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/Data/CombatType.h"
#include "AttackData.generated.h"

class UAnimMontage;

UCLASS(BlueprintType)
class ARUNTHECURSEDVEIL_API UAttackData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Identity")
	FName AttackID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Identity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Values")
	float BaseDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Values")
	float Range = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Values")
	float RecoveryTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Values")
	EAttackType AttackType = EAttackType::Light;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Targeting")
	float HitRadius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Targeting")
	int32 MaxHits = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Targeting")
	bool bIgnoreBlocks = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Animation")
	FName HitSocketName = NAME_None;
};
