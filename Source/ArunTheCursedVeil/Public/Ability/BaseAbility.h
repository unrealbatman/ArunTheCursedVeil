#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/ArunTypes.h"
#include "BaseAbility.generated.h"

class AArun;
class UCombatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityDeactivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityCooldownStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityCooldownFinished);

UCLASS(ClassGroup = (Abilities), Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class ARUNTHECURSEDVEIL_API UBaseAbility : public UActorComponent
{
	GENERATED_BODY()

public:

	UBaseAbility();

protected:

	virtual void BeginPlay() override;

#pragma region Ability Settings

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FName AbilityID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FText AbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float CooldownDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float AbilityDuration = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	bool bBossFightOnly = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	bool bStartsUnlocked = true;

#pragma endregion

#pragma region Runtime

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	EAbilityState AbilityState = EAbilityState::Ready;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	bool bUnlocked = true;

	UPROPERTY()
	TObjectPtr<AArun> OwnerArun;

	// Phase 9: abilities request the Ability combat state through this, they never set it directly.
	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComp;

	FTimerHandle AbilityDurationTimerHandle;

	FTimerHandle CooldownTimerHandle;

#pragma endregion

#pragma region Internal

	void HandleAbilityDurationFinished();

	void HandleCooldownFinished();

	void StartCooldown();

#pragma endregion

public:

#pragma region Delegates

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityActivated OnAbilityActivated;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityDeactivated OnAbilityDeactivated;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityCooldownStarted OnAbilityCooldownStarted;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityCooldownFinished OnAbilityCooldownFinished;

#pragma endregion

#pragma region Ability

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual bool ActivateAbility();

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual void DeactivateAbility();

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool CanActivateAbility() const;

#pragma endregion

#pragma region Blueprint Events

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void BP_OnAbilityActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void BP_OnAbilityDeactivated();

#pragma endregion

#pragma region Utility

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void UnlockAbility();

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void LockAbility();

	UFUNCTION(BlueprintPure, Category = "Ability")
	bool IsUnlocked() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	bool IsOnCooldown() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	bool IsAbilityActive() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	EAbilityState GetAbilityState() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetCooldownDuration() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetAbilityDuration() const;

#pragma endregion
};