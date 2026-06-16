#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Arun.generated.h"

class USpringArmComponent;
class UCameraComponent;

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

public:

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void ActivateUnderworldMode();

	UFUNCTION(BlueprintCallable)
	void DeactivateUnderworldMode();

	FORCEINLINE UCameraComponent* GetFollowCamera() const
	{
		return FollowCamera;
	}

	FORCEINLINE USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom;
	}
};