#include "Combat/Components/CombatComponent.h"
#include "Combat/Components/TargetingComponent.h"
#include "Combat/Data/AttackData.h"
#include "Combat/Systems/DamageSystem.h"
#include "Component/DarkImpulseComponent.h"
#include "Component/HealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogArunCombat, Log, All);

// Testing helper: prints to both the output log and the screen when bLogCombatEvents is on,
// so you can watch the pipeline fire without needing an anim montage/notify set up yet.
#define ARUN_COMBAT_LOG(Format, ...) \
    if (bLogCombatEvents) \
    { \
        const FString Arun_CombatLogMsg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
        UE_LOG(LogArunCombat, Log, TEXT("[%s] %s"), *GetNameSafe(GetOwner()), *Arun_CombatLogMsg); \
        if (GEngine) \
        { \
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("[Combat:%s] %s"), *GetNameSafe(GetOwner()), *Arun_CombatLogMsg)); \
        } \
    }

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // Architecture constraint: event-driven, no hot ticks!

    CurrentState = ECombatState::Idle;
    LockedTarget = nullptr;
    ComboCount = 0;
    StateStartTime = 0.0f;
    HealthComp = nullptr;
    ImpulseComp = nullptr;
    TargetingComp = nullptr;

    DamageSystem = CreateDefaultSubobject<UDamageSystem>(TEXT("DamageSystem"));
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();

    if (const AActor* Owner = GetOwner())
    {
        // Safely capture component links dynamically on startup
        HealthComp = Owner->FindComponentByClass<UHealthComponent>();
        ImpulseComp = Owner->FindComponentByClass<UDarkImpulseComponent>();
        TargetingComp = Owner->FindComponentByClass<UTargetingComponent>();

        if (HealthComp)
        {
            HealthComp->OnDeath.AddDynamic(this, &UCombatComponent::HandleOwnerDeath);

            if (HealthComp->IsDead())
            {
                HandleOwnerDeath();
            }
        }

        if (ImpulseComp)
        {
            ImpulseComp->OnUnderworldActivated.AddDynamic(this, &UCombatComponent::HandleUnderworldActivated);
            ImpulseComp->OnUnderworldDeactivated.AddDynamic(this, &UCombatComponent::HandleUnderworldDeactivated);
        }
    }
}

void UCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ClearCombatTimers();

    if (HealthComp)
    {
        HealthComp->OnDeath.RemoveDynamic(this, &UCombatComponent::HandleOwnerDeath);
    }

    if (ImpulseComp)
    {
        ImpulseComp->OnUnderworldActivated.RemoveDynamic(this, &UCombatComponent::HandleUnderworldActivated);
        ImpulseComp->OnUnderworldDeactivated.RemoveDynamic(this, &UCombatComponent::HandleUnderworldDeactivated);
    }

    Super::EndPlay(EndPlayReason);
}

bool UCombatComponent::CanAttack() const
{
    // Layered constraint validation: Strict evaluation of execution window
    if (IsDead())
    {
        return false;
    }

    return (CurrentState == ECombatState::Idle || CurrentState == ECombatState::Underworld);
}

bool UCombatComponent::CanReceiveHit() const
{
    // Cannot stagger or hit characters that are already dead
    return !IsDead();
}

bool UCombatComponent::CanBlock() const
{
    if (IsDead())
    {
        return false;
    }

    return CurrentState == ECombatState::Idle || CurrentState == ECombatState::Underworld || CurrentState == ECombatState::Blocking;
}

bool UCombatComponent::IsDead() const
{
    return CurrentState == ECombatState::Dead || (HealthComp && HealthComp->IsDead());
}

float UCombatComponent::GetStateElapsedTime() const
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return 0.0f;
    }

    return FMath::Max(0.0f, World->GetTimeSeconds() - StateStartTime);
}

void UCombatComponent::Attack(const FCombatAttackRequest& InAttackRequest)
{
    StartAttack(InAttackRequest, EAttackType::Light);
}

void UCombatComponent::HeavyAttack(const FCombatAttackRequest& InAttackRequest)
{
    StartAttack(InAttackRequest, EAttackType::Heavy);
}

void UCombatComponent::Cancel()
{
    if (IsDead())
    {
        return;
    }

    const bool bWasAttackActive = CurrentState == ECombatState::Attacking || CurrentState == ECombatState::Recover;
    ClearCombatTimers();
    TransitionToState(ECombatState::Idle);

    if (bWasAttackActive)
    {
        OnAttackEnded.Broadcast();
    }
}

void UCombatComponent::LockTarget(AActor* InTarget)
{
    if (LockedTarget == InTarget)
    {
        return;
    }

    LockedTarget = InTarget;
    OnTargetChanged.Broadcast(LockedTarget.Get());
}

void UCombatComponent::ClearTarget()
{
    LockTarget(nullptr);
}

void UCombatComponent::TakeHit(const FHitContext& InHitContext)
{
    if (!CanReceiveHit())
    {
        ARUN_COMBAT_LOG("TakeHit ignored - CanReceiveHit() is false (likely already dead)");
        return;
    }

    ARUN_COMBAT_LOG("TakeHit from %s - %.1f damage", *GetNameSafe(InHitContext.Attacker.Get()), InHitContext.BaseDamage);

    ClearCombatTimers();
    TransitionToState(ECombatState::Hit);
}

void UCombatComponent::Block(const FBlockContext& InBlockContext)
{
    if (!CanBlock())
    {
        return;
    }

    TransitionToState(ECombatState::Blocking);
    OnBlocked.Broadcast(InBlockContext.Blocker.Get());
}

void UCombatComponent::TransitionToState(ECombatState NewState)
{
    if (CurrentState == NewState) return;

    ECombatState OldState = CurrentState;
    CurrentState = NewState;

    if (GetWorld())
    {
        StateStartTime = GetWorld()->GetTimeSeconds();
    }

    ARUN_COMBAT_LOG("State: %s -> %s", *UEnum::GetValueAsString(OldState), *UEnum::GetValueAsString(NewState));

    // Inform outer systems completely decoupled via execution pipeline
    if (OnStateChanged.IsBound())
    {
        OnStateChanged.Broadcast(OldState, NewState, StateStartTime);
    }
}

void UCombatComponent::StartAttack(const FCombatAttackRequest& InAttackRequest, EAttackType ForcedAttackType)
{
    if (!CanAttack())
    {
        ARUN_COMBAT_LOG("Attack rejected - CanAttack() is false in state %s", *UEnum::GetValueAsString(CurrentState));
        return;
    }

    ClearCombatTimers();

    LastAttackRequest = InAttackRequest;
    LastAttackRequest.AttackType = ForcedAttackType;
    HitActorsThisAttack.Reset();

    const FString AttackDataName = LastAttackRequest.AttackData ? LastAttackRequest.AttackData->GetName() : TEXT("<none>");
    ARUN_COMBAT_LOG("Attack started - Type=%s AttackData=%s", *UEnum::GetValueAsString(ForcedAttackType), *AttackDataName);

    TransitionToState(ECombatState::Attacking);
    OnAttackStarted.Broadcast();

    // Testing aid: with no montage, no anim notify will ever call PerformHitFrame, so simulate
    // the hit frame here after a short delay instead. Remove/disable once real montages exist.
    if (AutoHitFrameDelay > 0.0f && (!LastAttackRequest.AttackData || !LastAttackRequest.AttackData->AnimMontage))
    {
        if (UWorld* World = GetWorld())
        {
            ARUN_COMBAT_LOG("No AnimMontage on this attack - auto-triggering hit frame in %.2fs", AutoHitFrameDelay);

            World->GetTimerManager().SetTimer(
                HitFrameTimerHandle,
                [this]() { PerformHitFrame(NAME_None); },
                AutoHitFrameDelay,
                false);
        }
    }

    BeginRecovery(ResolveRecoveryTime());
}

void UCombatComponent::BeginRecovery(float RecoveryTime)
{
    if (IsDead())
    {
        return;
    }

    TransitionToState(ECombatState::Recover);

    if (RecoveryTime <= 0.0f)
    {
        FinishRecovery();
        return;
    }

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            RecoveryTimerHandle,
            this,
            &UCombatComponent::FinishRecovery,
            RecoveryTime,
            false);
    }
    else
    {
        FinishRecovery();
    }
}

void UCombatComponent::FinishRecovery()
{
    if (IsDead())
    {
        return;
    }

    ARUN_COMBAT_LOG("Recovery finished - back to Idle");

    ClearCombatTimers();
    TransitionToState(ECombatState::Idle);
    OnAttackEnded.Broadcast();
}

float UCombatComponent::ResolveRecoveryTime() const
{
    float RecoveryTime = LastAttackRequest.AttackData
        ? FMath::Max(0.0f, LastAttackRequest.AttackData->RecoveryTime)
        : 0.5f;

    // Underworld/DarkImpulse corruption speeds up recovery the deeper Arun falls (spec 8.3).
    if (ImpulseComp)
    {
        const float AttackSpeedMultiplier = 1.0f + (ImpulseComp->GetDarkImpulse() / 100.0f * 0.3f);
        RecoveryTime /= AttackSpeedMultiplier;
    }

    return RecoveryTime;
}

void UCombatComponent::ClearCombatTimers()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(RecoveryTimerHandle);
        World->GetTimerManager().ClearTimer(HitFrameTimerHandle);
    }
}

void UCombatComponent::PerformHitFrame(FName SocketName)
{
    // Only the attacker's own hit frame matters, and only while actively attacking.
    if (CurrentState != ECombatState::Attacking)
    {
        ARUN_COMBAT_LOG("PerformHitFrame ignored - not in Attacking state (currently %s)", *UEnum::GetValueAsString(CurrentState));
        return;
    }

    UAttackData* AttackData = LastAttackRequest.AttackData;
    if (!AttackData)
    {
        ARUN_COMBAT_LOG("PerformHitFrame aborted - no AttackData on the active attack");
        return;
    }

    if (!TargetingComp)
    {
        ARUN_COMBAT_LOG("PerformHitFrame aborted - no TargetingComponent found on owner (add one in the Blueprint)");
        return;
    }

    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return;
    }

    FVector TraceOrigin = Owner->GetActorLocation();
    if (const ACharacter* OwnerCharacter = Cast<ACharacter>(Owner))
    {
        if (USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh())
        {
            if (SocketName != NAME_None && Mesh->DoesSocketExist(SocketName))
            {
                TraceOrigin = Mesh->GetSocketLocation(SocketName);
            }
        }
    }

    TArray<FHitResult> Hits;
    TargetingComp->TraceForTargets(TraceOrigin, AttackData->HitRadius, Owner, Hits);

    ARUN_COMBAT_LOG("Hit frame trace at %s, radius %.0f - %d overlap(s) found", *TraceOrigin.ToCompactString(), AttackData->HitRadius, Hits.Num());

    int32 ConfirmedCount = 0;

    for (const FHitResult& Hit : Hits)
    {
        if (HitActorsThisAttack.Num() >= AttackData->MaxHits)
        {
            ARUN_COMBAT_LOG("MaxHits (%d) reached for this attack - stopping", AttackData->MaxHits);
            break;
        }

        AActor* Target = Hit.GetActor();
        if (!Target || HitActorsThisAttack.Contains(Target) || !TargetingComp->IsValidTarget(Target))
        {
            continue;
        }

        HitActorsThisAttack.Add(Target);
        ++ConfirmedCount;

        ARUN_COMBAT_LOG("Hit confirmed on %s", *GetNameSafe(Target));

        OnHitConfirmed.Broadcast(Hit);

        ResolveHitOnTarget(Target, Hit);
    }

    if (ConfirmedCount == 0)
    {
        ARUN_COMBAT_LOG("No valid targets in range - check TargetingComponent's TraceChannel and that the target has a HealthComponent");
    }
}

void UCombatComponent::ResolveHitOnTarget(AActor* Target, const FHitResult& Hit)
{
    UAttackData* AttackData = LastAttackRequest.AttackData;
    if (!AttackData || !DamageSystem || !Target)
    {
        return;
    }

    UCombatComponent* TargetCombat = Target->FindComponentByClass<UCombatComponent>();
    const bool bWasBlocked = !AttackData->bIgnoreBlocks && TargetCombat && TargetCombat->IsBlocking();

    // Combat re-queries DarkImpulse modifiers on every attack rather than caching them (per spec 8.4).
    float DamageMultiplier = 1.0f;
    if (ImpulseComp)
    {
        DamageMultiplier = 1.0f + (ImpulseComp->GetDarkImpulse() / 100.0f * 0.5f);
    }

    FDamageContext DamageContext;
    DamageContext.Source = GetOwner();
    DamageContext.Target = Target;
    DamageContext.BaseDamage = AttackData->BaseDamage;
    DamageContext.DamageMultiplier = DamageMultiplier;
    DamageContext.bWasBlocked = bWasBlocked;
    DamageContext.AttackSource = AttackData;

    const FDamageResult Result = DamageSystem->ApplyDamage(DamageContext);

    ARUN_COMBAT_LOG("Damage resolved on %s - Outcome=%s FinalDamage=%.1f Blocked=%s",
        *GetNameSafe(Target), *UEnum::GetValueAsString(Result.Outcome), Result.FinalDamage, bWasBlocked ? TEXT("true") : TEXT("false"));

    if (Result.Outcome == EDamageOutcome::None)
    {
        ARUN_COMBAT_LOG("Damage NOT applied - target has no HealthComponent, or is already dead");
        return;
    }

    OnDamageApplied.Broadcast(Result.FinalDamage);

    if (TargetCombat && !bWasBlocked)
    {
        FHitContext HitContext;
        HitContext.Attacker = GetOwner();
        HitContext.Defender = Target;
        HitContext.HitLocation = Hit.Location;
        HitContext.HitNormal = Hit.Normal;
        HitContext.BaseDamage = Result.FinalDamage;
        HitContext.AttackSource = AttackData;

        TargetCombat->TakeHit(HitContext);
    }
}

void UCombatComponent::HandleOwnerDeath()
{
    if (CurrentState == ECombatState::Dead)
    {
        return;
    }

    ARUN_COMBAT_LOG("Owner died - transitioning to Dead state");

    ClearCombatTimers();
    ClearTarget();
    TransitionToState(ECombatState::Dead);
    OnDeath.Broadcast();
}

bool UCombatComponent::TryEnterAbilityState()
{
    if (!CanAttack())
    {
        return false;
    }

    ClearCombatTimers();
    TransitionToState(ECombatState::Ability);
    return true;
}

void UCombatComponent::ExitAbilityState()
{
    if (CurrentState != ECombatState::Ability)
    {
        return;
    }

    TransitionToState(ECombatState::Idle);
}

void UCombatComponent::HandleUnderworldActivated()
{
    // Only enter the Underworld state from a neutral state - never interrupt an attack, hit, or death.
    if (CurrentState == ECombatState::Idle)
    {
        TransitionToState(ECombatState::Underworld);
    }
}

void UCombatComponent::HandleUnderworldDeactivated()
{
    if (CurrentState == ECombatState::Underworld)
    {
        TransitionToState(ECombatState::Idle);
    }
}
