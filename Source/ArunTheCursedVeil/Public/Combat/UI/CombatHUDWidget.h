#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Combat/Data/CombatType.h"
#include "CombatHUDWidget.generated.h"

class UHealthComponent;
class UCombatComponent;
class UDarkImpulseComponent;

/**
 * UCombatHUDWidget - Phase 7 UI gateway.
 * Pulls state from Health/Combat/DarkImpulse components purely by listening to
 * their events. Combat/Health/DarkImpulse never know this widget exists.
 * Subclass this as a Widget Blueprint and implement the BP_On... events.
 */
UCLASS()
class ARUNTHECURSEDVEIL_API UCombatHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Call once (e.g. from the HUD/GameMode) after the widget is created, passing the Pawn to track. */
	UFUNCTION(BlueprintCallable, Category = "Combat|UI")
	void InitializeForPawn(APawn* Pawn);

protected:
	UFUNCTION()
	void HandleHealthChanged(float NewHealth, float MaxHealth);

	UFUNCTION()
	void HandleDeath();

	UFUNCTION()
	void HandleDarkImpulseChanged(float NewValue);

	UFUNCTION()
	void HandleHumanityChanged(float NewValue);

	UFUNCTION()
	void HandleCombatStateChanged(ECombatState OldState, ECombatState NewState, float TimeStamp);

	UPROPERTY(BlueprintReadOnly, Category = "Combat|UI")
	TObjectPtr<UHealthComponent> BoundHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Combat|UI")
	TObjectPtr<UCombatComponent> BoundCombat;

	UPROPERTY(BlueprintReadOnly, Category = "Combat|UI")
	TObjectPtr<UDarkImpulseComponent> BoundImpulse;

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat|UI")
	void BP_OnHealthChanged(float NewHealth, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat|UI")
	void BP_OnDeath();

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat|UI")
	void BP_OnDarkImpulseChanged(float NewValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat|UI")
	void BP_OnHumanityChanged(float NewValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat|UI")
	void BP_OnCombatStateChanged(ECombatState OldState, ECombatState NewState);
};
