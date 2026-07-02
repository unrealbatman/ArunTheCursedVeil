#include "Combat/UI/CombatHUDWidget.h"
#include "Combat/Components/CombatComponent.h"
#include "Component/HealthComponent.h"
#include "Component/DarkImpulseComponent.h"
#include "GameFramework/Pawn.h"

void UCombatHUDWidget::InitializeForPawn(APawn* Pawn)
{
	if (!Pawn)
	{
		return;
	}

	BoundHealth = Pawn->FindComponentByClass<UHealthComponent>();
	BoundCombat = Pawn->FindComponentByClass<UCombatComponent>();
	BoundImpulse = Pawn->FindComponentByClass<UDarkImpulseComponent>();

	if (BoundHealth)
	{
		BoundHealth->OnHealthChanged.AddDynamic(this, &UCombatHUDWidget::HandleHealthChanged);
		BoundHealth->OnDeath.AddDynamic(this, &UCombatHUDWidget::HandleDeath);

		HandleHealthChanged(BoundHealth->GetCurrentHealth(), BoundHealth->GetMaxHealth());
	}

	if (BoundCombat)
	{
		BoundCombat->OnStateChanged.AddDynamic(this, &UCombatHUDWidget::HandleCombatStateChanged);
	}

	if (BoundImpulse)
	{
		BoundImpulse->OnDarkImpulseChanged.AddDynamic(this, &UCombatHUDWidget::HandleDarkImpulseChanged);
		BoundImpulse->OnHumanityChanged.AddDynamic(this, &UCombatHUDWidget::HandleHumanityChanged);

		HandleDarkImpulseChanged(BoundImpulse->GetDarkImpulse());
		HandleHumanityChanged(BoundImpulse->GetHumanity());
	}
}

void UCombatHUDWidget::HandleHealthChanged(float NewHealth, float MaxHealth)
{
	BP_OnHealthChanged(NewHealth, MaxHealth);
}

void UCombatHUDWidget::HandleDeath()
{
	BP_OnDeath();
}

void UCombatHUDWidget::HandleDarkImpulseChanged(float NewValue)
{
	BP_OnDarkImpulseChanged(NewValue);
}

void UCombatHUDWidget::HandleHumanityChanged(float NewValue)
{
	BP_OnHumanityChanged(NewValue);
}

void UCombatHUDWidget::HandleCombatStateChanged(ECombatState OldState, ECombatState NewState, float TimeStamp)
{
	BP_OnCombatStateChanged(OldState, NewState);
}
