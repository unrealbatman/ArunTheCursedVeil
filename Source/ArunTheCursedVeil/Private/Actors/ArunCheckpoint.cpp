#include "Actors/ArunCheckpoint.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

#include "Character/Arun.h"

#include "GameMode/ArunGameMode.h"

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

AArunCheckpoint::AArunCheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(
		TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("CheckpointMesh"));
	CheckpointMesh->SetupAttachment(SceneRoot);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(
		TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(SceneRoot);

	TriggerBox->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly);

	TriggerBox->SetGenerateOverlapEvents(true);

	bActivated = false;

	CheckpointID = NAME_None;

	CheckpointDisplayName = FText::FromString(
		TEXT("Checkpoint"));
}

void AArunCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(
			this,
			&AArunCheckpoint::OnTriggerBeginOverlap);
	}
}

void AArunCheckpoint::OnTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (bActivated)
	{
		return;
	}

	if (!OtherActor)
	{
		return;
	}

	AArun* ArunCharacter = Cast<AArun>(OtherActor);

	if (!ArunCharacter)
	{
		return;
	}

	ActivateCheckpoint();
}

void AArunCheckpoint::ActivateCheckpoint()
{
	if (bActivated)
	{
		return;
	}

	bActivated = true;

	AArunGameMode* ArunGameMode =
		Cast<AArunGameMode>(
			UGameplayStatics::GetGameMode(this));

	if (ArunGameMode)
	{
		ArunGameMode->SetCheckpoint(
			CheckpointID);
	}

	OnCheckpointActivated.Broadcast(
		CheckpointID);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			FString::Printf(
				TEXT("Checkpoint Activated: %s"),
				*CheckpointID.ToString()));
	}
}

void AArunCheckpoint::DeactivateCheckpoint()
{
	bActivated = false;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Yellow,
			FString::Printf(
				TEXT("Checkpoint Deactivated: %s"),
				*CheckpointID.ToString()));
	}
}

bool AArunCheckpoint::IsActivated() const
{
	return bActivated;
}

FName AArunCheckpoint::GetCheckpointID() const
{
	return CheckpointID;
}

FText AArunCheckpoint::GetCheckpointDisplayName() const
{
	return CheckpointDisplayName;
}