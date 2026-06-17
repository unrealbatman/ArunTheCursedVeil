#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArunCheckpoint.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCheckpointActivated,
	FName,
	CheckpointID);

UCLASS()
class ARUNTHECURSEDVEIL_API AArunCheckpoint : public AActor
{
	GENERATED_BODY()

public:

	AArunCheckpoint();

protected:

	virtual void BeginPlay() override;

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> TriggerBox;

#pragma endregion

#pragma region Checkpoint Data

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	FName CheckpointID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	FText CheckpointDisplayName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	bool bActivated;

#pragma endregion

#pragma region Overlap

	UFUNCTION()
	void OnTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

#pragma endregion

public:

#pragma region Delegates

	UPROPERTY(BlueprintAssignable, Category = "Checkpoint")
	FOnCheckpointActivated OnCheckpointActivated;

#pragma endregion

#pragma region Functions

	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	void ActivateCheckpoint();

	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	void DeactivateCheckpoint();

#pragma endregion

#pragma region Getters

	UFUNCTION(BlueprintPure, Category = "Checkpoint")
	bool IsActivated() const;

	UFUNCTION(BlueprintPure, Category = "Checkpoint")
	FName GetCheckpointID() const;

	UFUNCTION(BlueprintPure, Category = "Checkpoint")
	FText GetCheckpointDisplayName() const;

#pragma endregion
};