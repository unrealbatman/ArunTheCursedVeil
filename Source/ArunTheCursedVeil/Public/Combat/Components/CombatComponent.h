#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/Data/CombatType.h"
#include "CombatComponent.generated.h"

// Forward declarations to keep dependencies loose
class UHealthComponent;
class UDarkImpulseComponent;
class UTargetingComponent;
class UDamageSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleCombatDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCombatStateChanged, ECombatState, OldState, ECombatState, NewState, float, TimeStamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatHitConfirmed, const FHitResult&, HitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatDamageApplied, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatTargetChanged, AActor*, NewTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatBlocked, AActor*, Blocker);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARUNTHECURSEDVEIL_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UCombatComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:    
    // Core Public API Contract
    UFUNCTION(BlueprintCallable, Category = "Combat|Actions")
    void Attack(const FCombatAttackRequest& InAttackRequest);

    UFUNCTION(BlueprintCallable, Category = "Combat|Actions")
    void HeavyAttack(const FCombatAttackRequest& InAttackRequest);

    UFUNCTION(BlueprintCallable, Category = "Combat|Actions")
    void Cancel();

    // State Queries
    UFUNCTION(BlueprintPure, Category = "Combat|State")
    bool CanAttack() const;

    UFUNCTION(BlueprintPure, Category = "Combat|State")
    bool CanReceiveHit() const;

    UFUNCTION(BlueprintPure, Category = "Combat|State")
    bool CanBlock() const;

    UFUNCTION(BlueprintPure, Category = "Combat|State")
    ECombatState GetCurrentState() const { return CurrentState; }

    UFUNCTION(BlueprintPure, Category = "Combat|State")
    bool IsAttacking() const { return CurrentState == ECombatState::Attacking; }

    UFUNCTION(BlueprintPure, Category = "Combat|State")
    bool IsRecovering() const { return CurrentState == ECombatState::Recover; }

    UFUNCTION(BlueprintPure, Category = "Combat|State")
    bool IsBlocking() const { return CurrentState == ECombatState::Blocking; }

    UFUNCTION(BlueprintPure, Category = "Combat|State")
    bool IsDead() const;

    UFUNCTION(BlueprintPure, Category = "Combat|State")
    float GetStateElapsedTime() const;

    // Targeting
    UFUNCTION(BlueprintCallable, Category = "Combat|Targeting")
    void LockTarget(AActor* InTarget);

    UFUNCTION(BlueprintCallable, Category = "Combat|Targeting")
    void ClearTarget();

    UFUNCTION(BlueprintPure, Category = "Combat|Targeting")
    AActor* GetLockedTarget() const { return LockedTarget.Get(); }

    // Hit Reception
    UFUNCTION(BlueprintCallable, Category = "Combat|Hit")
    void TakeHit(const FHitContext& InHitContext);

    UFUNCTION(BlueprintCallable, Category = "Combat|Hit")
    void Block(const FBlockContext& InBlockContext);

    // State Transitions
    void TransitionToState(ECombatState NewState);

    // Called by UAnimNotify_CombatHitFrame at the animation-specified hit frame (Phase 5).
    UFUNCTION(BlueprintCallable, Category = "Combat|Hit")
    void PerformHitFrame(FName SocketName);

    // Ability integration (Phase 9): abilities request the Ability state through combat,
    // they never assign CurrentState directly.
    UFUNCTION(BlueprintCallable, Category = "Combat|Ability")
    bool TryEnterAbilityState();

    UFUNCTION(BlueprintCallable, Category = "Combat|Ability")
    void ExitAbilityState();

    UFUNCTION(BlueprintPure, Category = "Combat|State")
    UAttackData* GetActiveAttackData() const { return LastAttackRequest.AttackData; }

    // Architecture Event Contracts
    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FSimpleCombatDelegate OnAttackStarted;

    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FSimpleCombatDelegate OnAttackEnded;

    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FOnCombatStateChanged OnStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FOnCombatHitConfirmed OnHitConfirmed;

    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FOnCombatDamageApplied OnDamageApplied;

    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FOnCombatTargetChanged OnTargetChanged;

    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FOnCombatBlocked OnBlocked;

    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FSimpleCombatDelegate OnDeath;

public:
    // Testing aid: if the active attack has no AnimMontage (so no anim notify will ever fire),
    // PerformHitFrame() is auto-triggered this many seconds after the attack starts. Lets you
    // verify the full trace/damage pipeline before any animation work is done. Set to 0 to disable.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Debug")
    float AutoHitFrameDelay = 0.15f;

    // Testing aid: prints attack/state/hit/damage events via UE_LOG (category LogArunCombat)
    // and on-screen debug messages.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Debug")
    bool bLogCombatEvents = true;

private:
    void StartAttack(const FCombatAttackRequest& InAttackRequest, EAttackType ForcedAttackType);
    void BeginRecovery(float RecoveryTime);
    void FinishRecovery();
    float ResolveRecoveryTime() const;
    void ClearCombatTimers();

    void ResolveHitOnTarget(AActor* Target, const FHitResult& Hit);

    UFUNCTION()
    void HandleOwnerDeath();

    UFUNCTION()
    void HandleUnderworldActivated();

    UFUNCTION()
    void HandleUnderworldDeactivated();

    // Cached references
    UPROPERTY(VisibleAnywhere, Category = "Combat|Internal")
    UHealthComponent* HealthComp;

    UPROPERTY(VisibleAnywhere, Category = "Combat|Internal")
    UDarkImpulseComponent* ImpulseComp;

    UPROPERTY(VisibleAnywhere, Category = "Combat|Internal")
    UTargetingComponent* TargetingComp;

    UPROPERTY(VisibleAnywhere, Category = "Combat|Internal")
    TObjectPtr<UDamageSystem> DamageSystem;

    // Targets already hit during the current attack (enforces AttackData->MaxHits).
    UPROPERTY(Transient)
    TArray<TObjectPtr<AActor>> HitActorsThisAttack;

    // Tracked internal states
    UPROPERTY(VisibleAnywhere, Category = "Combat|State")
    ECombatState CurrentState;

    UPROPERTY(VisibleAnywhere, Category = "Combat|State")
    TObjectPtr<AActor> LockedTarget;

    UPROPERTY(VisibleAnywhere, Category = "Combat|State")
    FCombatAttackRequest LastAttackRequest;

    UPROPERTY(VisibleAnywhere, Category = "Combat|State")
    float StateStartTime;

    UPROPERTY(VisibleAnywhere, Category = "Combat|State")
    int32 ComboCount;

    FTimerHandle RecoveryTimerHandle;
    FTimerHandle HitFrameTimerHandle;
};
