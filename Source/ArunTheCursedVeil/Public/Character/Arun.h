#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Arun.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCombatComponent;
class UAttackData;

UCLASS()
class ARUNTHECURSEDVEIL_API AArun : public ACharacter
{
	GENERATED_BODY()

public:
	AArun();

protected:
	virtual void BeginPlay() override;

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FollowCamera;

#pragma endregion

#pragma region Character State

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bUnderworldMode = false;

#pragma endregion

#pragma region Combat

	// Assigned in the editor - which UAttackData asset each input maps to.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UAttackData> LightAttackData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UAttackData> HeavyAttackData;

private:
	// Cached on BeginPlay - CombatComponent is added in the character Blueprint, not in C++.
	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComp;

#pragma endregion

public:

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void ActivateUnderworldMode();

	UFUNCTION(BlueprintCallable)
	void DeactivateUnderworldMode();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformLightAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformHeavyAttack();

	FORCEINLINE UCameraComponent* GetFollowCamera() const
	{
		return FollowCamera;
	}

	FORCEINLINE USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom;
	}

	FORCEINLINE UCombatComponent* GetCombatComponent() const
	{
		return CombatComp;
	}
};