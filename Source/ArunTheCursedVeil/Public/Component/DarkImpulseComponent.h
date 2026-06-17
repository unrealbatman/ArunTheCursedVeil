#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/ArunTypes.h"
#include "DarkImpulseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnDarkImpulseChanged,
	float,
	NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnHumanityChanged,
	float,
	NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCorruptionStageChanged,
	ECorruptionStage,
	NewStage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnUnderworldActivated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnUnderworldDeactivated);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARUNTHECURSEDVEIL_API UDarkImpulseComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UDarkImpulseComponent();

protected:

	virtual void BeginPlay() override;

#pragma region Core Values

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dark Impulse")
	float DarkImpulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dark Impulse")
	float Humanity = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dark Impulse")
	ECorruptionStage CorruptionStage = ECorruptionStage::Pure;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dark Impulse")
	bool bUnderworldModeActive = false;

#pragma endregion

#pragma region Internal Functions

	void UpdateCorruptionStage();

	void BroadcastDarkImpulseUpdate();

	void BroadcastHumanityUpdate();

#pragma endregion

public:

#pragma region Delegates

	UPROPERTY(BlueprintAssignable, Category = "Dark Impulse")
	FOnDarkImpulseChanged OnDarkImpulseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Dark Impulse")
	FOnHumanityChanged OnHumanityChanged;

	UPROPERTY(BlueprintAssignable, Category = "Dark Impulse")
	FOnCorruptionStageChanged OnCorruptionStageChanged;

	UPROPERTY(BlueprintAssignable, Category = "Dark Impulse")
	FOnUnderworldActivated OnUnderworldActivated;

	UPROPERTY(BlueprintAssignable, Category = "Dark Impulse")
	FOnUnderworldDeactivated OnUnderworldDeactivated;

#pragma endregion

#pragma region Dark Impulse

	UFUNCTION(BlueprintCallable, Category = "Dark Impulse")
	void AddDarkImpulse(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Dark Impulse")
	void ReduceDarkImpulse(float Amount);

#pragma endregion

#pragma region Humanity

	UFUNCTION(BlueprintCallable, Category = "Dark Impulse")
	void AddHumanity(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Dark Impulse")
	void ReduceHumanity(float Amount);

#pragma endregion

#pragma region Underworld

	UFUNCTION(BlueprintCallable, Category = "Dark Impulse")
	void ActivateUnderworldMode();

	UFUNCTION(BlueprintCallable, Category = "Dark Impulse")
	void DeactivateUnderworldMode();

#pragma endregion

#pragma region Getters

	UFUNCTION(BlueprintPure, Category = "Dark Impulse")
	float GetDarkImpulse() const;

	UFUNCTION(BlueprintPure, Category = "Dark Impulse")
	float GetHumanity() const;

	UFUNCTION(BlueprintPure, Category = "Dark Impulse")
	float GetDarkImpulsePercent() const;

	UFUNCTION(BlueprintPure, Category = "Dark Impulse")
	float GetHumanityPercent() const;

	UFUNCTION(BlueprintPure, Category = "Dark Impulse")
	ECorruptionStage GetCorruptionStage() const;

	UFUNCTION(BlueprintPure, Category = "Dark Impulse")
	bool IsUnderworldModeActive() const;

#pragma endregion
};