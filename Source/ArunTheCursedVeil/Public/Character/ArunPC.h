#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "ArunPC.generated.h"  

class UInputMappingContext;
class UInputAction;

UENUM(BlueprintType)
enum class ECameraState : uint8
{
	Exploration	UMETA(DisplayName = "Exploration"),
	Combat		UMETA(DisplayName = "Combat"),
	LockOn		UMETA(DisplayName = "Lock On"),
	Cinematic	UMETA(DisplayName = "Cinematic")
};

UCLASS()
class ARUNTHECURSEDVEIL_API AArunPC : public APlayerController
{
	GENERATED_BODY()

public:
	AArunPC();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

#pragma region InputActions

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LockOnAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> UnderworldModeAction;

#pragma endregion

#pragma region InputFunctions

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void JumpPressed();
	void JumpReleased();

	void SprintPressed();
	void SprintReleased();

	void PausePressed();

	void LockOnPressed();

	void ActivateUnderworldMode();

#pragma endregion

public:

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	ECameraState CurrentCameraState = ECameraState::Exploration;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bLockOnActive = false;
};