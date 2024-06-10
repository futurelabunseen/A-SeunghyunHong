// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GOCharacterBase.h"
#include "Interface/GOCharacterHUDInterface.h"
#include "Interface/GOPlaySkillAnimInterface.h"
#include "Interface/GOSpellFlashInterface.h"
#include "Interface/GOPlaySkillEffectInterface.h"
#include "Interface/GOApplySkillInterface.h"
#include "Interface/GOPlayerInterface.h"
#include "Share/ShareEnums.h" 
#include "GameData/GOCharacterDataAsset.h"
#include "GameData/GOCharacterStat.h"
#include "Share/EGOPlayerActionState.h"
#include "Share/GOOrder.h"
#include "Share/EGOTeam.h"
#include "GOPlayerCharacter.generated.h"

struct FInputActionValue;
class UCommonInputSubsystem;
class UInputMappingContext;
class UInputAction;
class UNiagaraSystem;
class USpringArmComponent;
class UCameraComponent;
class USplineComponent;
class UGOHUDWidget;
class UGOSkillCastComponent;

// UCLASS(config = GuardiansOrders)
UCLASS()
class GUARDIANSORDERS_API AGOPlayerCharacter : public AGOCharacterBase, public IGOApplySkillInterface, public IGOCharacterHUDInterface, public IGOPlaySkillAnimInterface, public IGOSpellFlashInterface, public IGOPlaySkillEffectInterface, public IGOPlayerInterface
{
	GENERATED_BODY()
	
public:
	AGOPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	// virtual void SetDead() override;
	virtual void PossessedBy(AController* NewController) override;

	/**
	Owner의 값이 변경되면 호출되는 함수입니다.
	*/
	virtual void OnRep_Owner() override;

	virtual void PostNetInit() override;

// Data Section
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Data")
	EHeroType MyHeroType;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero Data")
	//UGOCharacterDataAsset* HeroDataAsset;

	//EHeroType HeroType;
	//ERoleType RoleType;
	//EAttackRange AttackRange;
	//EArchetype Archetype;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Input Section
protected:
	UPROPERTY()
	TObjectPtr<UCommonInputSubsystem> InputSubsystem;

	// Input-Move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> HeroMoveMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionGamePadMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionClickSetDestination;

	// Input-Skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> HeroSkillMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionBaseSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionSkillQ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionSkillW;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionSkillE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionSkillR;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionSpellD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionSpellF;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionSpellG;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActionShowMaxBasicAttackRange;


	void MoveGamePad(const FInputActionValue& Value);

	// Input handlers for SetDestination action
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	virtual void OnBaseSkill();
	virtual void OnSkillQ();
	virtual void OnSkillW();
	virtual void OnSkillE();
	virtual void OnSkillR();

	/**
	 * Flash Spell : 마우스 커서가 있는 쪽으로 짧은 순간이동이 가능합니다.
	 */
	void OnSpellD();
	void OnSpellF();
	void OnSpellG();	

	// 최대 공격 사거리를 보여주는 콜백 함수입니다. 
	void OnShowMaxBasicAttackRange();
public:
	void UpdateSkillBar();

public:
	// Time Threshold to know if it was a short press.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXImpact;

protected:
	FVector CachedDestination = FVector::ZeroVector;

	// For how long it has been pressed to follow mouse cursor.
	float FollowTime = 0.f;

	//True if the controlled character should navigate to the mouse cursor.
	bool bSelfMove = false;

	// Distance to stop AutoRun when close to Destination.
	UPROPERTY(EditDefaultsOnly)
	float SelfMoveAcceptanceRadius = 50.f;

	// Use for smooth interpolation.
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path Visualization")
	UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path Visualization")
	UMaterial* PathMaterial;

	TArray<class USplineMeshComponent*> SplineMeshComponents;

	float SplineMeshTickness = 0.1;

	void SelfMove();

private:
	UPROPERTY(Transient)
	TObjectPtr<UDecalComponent> BasicAttackRangeDecal;

	bool bIsDecalVisible;

// UI Section
protected:
	virtual void SetupHUDWidget(UGOHUDWidget* InHUDWidget) override;

// Skill Section
protected:
	// My Skill System
	
	// Skill Cast Component
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGOSkillCastComponent* SkillCastComponent;*/

	// Multiplayer lecture

	/**
	* 채널이 활성화되는 시작부터 끝까지 프로퍼티 리플리케이션이 활성화됩니다.
	*/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	* 공격과 애니메이션을 재생해주는 함수를 별도로 분리해서 구현합니다.
	*/
	void Attack();
	void PlayAttackAnimation();

	/**
	* IGOAnimationAttackInterface
	* 공격을 판정하는 함수입니다. 
	*/
	virtual void AttackHitCheck() override;

	// 새로 만든: 스킬시스템용 
	virtual void SkillAttackHitCheck() override;

	/**
	* 공격을 확정합니다.
	*/
	void AttackHitConfirm(AActor* HitActor);

	// 새로 만든: 스킬시스템용 
	void SkillHitConfirm(AActor* HitActor, float SkillAffectAmount, ESkillAffectType SkillAffectType);

	//void AttackHitConfirm(AActor* HitActor, float Damage);

	//// LineTraceMulti 또는 SweepMulti에서 오는 여러 히트 결과 처리
	//void AttackHitConfirm(const TArray<FHitResult>& HitResults, float Damage);

	//// OverlapMulti에서 오는 여러 오버랩 결과 처리
	//void AttackHitConfirm(const TArray<FOverlapResult>& OverlapResults, float Damage);

	void DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);

	/**
	* (1)
	* 공격 입력이 들어오면 클라이언트가 처리하는 것이 아니라 서버에 명령을 보냅니다.
	* 공격이 시작되는 시간 값을 인자로 받습니다.
	* 현재 내가 공격한 시간을 서버에 전송합니다.
	* 클라이언트의 월드와 서버의 월드는 다르므로 클라이언트는 서버의 시간을 가져와서 넘겨줘야 합니다.
	*/
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCAttack(float AttackStartTime);


	// 새로 만든: 스킬시스템용 구조체
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCActivateSkill(float AttackStartTime, FHeroSkillKey Key);

	/**
	* 
	*/
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAttack();	
	
	// 새로 만든: 스킬시스템용 구조체 !!!
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCActivateSkill(FHeroSkillKey Key);

public:
	UFUNCTION(Server, Reliable)
	void ServerRPCActivateSkillWithParticles(FHeroSkillKey Key);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCActivateSkillWithParticles(FHeroSkillKey Key);

	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayAnimation(AGOPlayerCharacter* CharacterToPlay);	
	
	// 새로 만든: 스킬시스템용 
	UFUNCTION(Client, Reliable)
	void ClientRPCPlaySkillAnimation(AGOPlayerCharacter* CharacterToPlay, UGOSkillBase* CurrentSkill);
	//
	// 새로 만든: 스킬시스템용 구조체
	UFUNCTION(Client, Reliable)
	void ClientRPCActivateSkill(AGOPlayerCharacter* CharacterToPlay, FHeroSkillKey Key);
	
	/**
	* 클라이언트가 무언가 액터에 맞았을 때 서버와 모든 클라이언트에게 판정 명령을 보냅니다.
	*/
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyHit(const FHitResult& HitResult, float HitChecktime);

	/**
	* 공격 판정 시 Miss 일 때의 결과를 보냅니다.
	*/
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyMiss(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime);

	/**
	 * 새로 만든: 스킬시스템용 
	 */
	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRPCNotifySkillHit(const FGOOutHitCollisionStructure SkillHitCollisionStructure, float HitChecktime, ESkillCollisionType CurrentSkillCollisionType, float DamageAmount);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifySkillMiss(float HitCheckTime);
	
	// 테스트용: 스킬시스템 FHitResult
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifySkillHitTest(const FHitResult& HitResult, float DamageAmount, ESkillAffectType SkillAffectType, FHeroSkillKey Key);

	// 테스트용: 스킬시스템 TArray<FHitResult>
	UFUNCTION(Server, Reliable)
	void ServerRPCNotifySkillHitResults(const TArray<FHitResult>& HitResults, float DamageAmount, ESkillAffectType SkillAffectType, FHeroSkillKey Key);

	// 테스트용: 스킬시스템 TArray<FOverlapResult>
	UFUNCTION(Server, Reliable)
	void ServerRPCNotifySkillHitOverlapResult(const TArray<FOverlapResult>& FOverlapResults, float DamageAmount, ESkillAffectType SkillAffectType, FHeroSkillKey Key);

	/** 
	* 현재 공격 중인가 ? 
	* 공격 중이라면 공격을 못하도록 플래그를 걸어주는 용도입니다.
	* 프로퍼티로 승격시켜서, 서버에서 이 값을 바꿀 때 (업데이트될 때) 자동으로 모든 클라이언트에게 바뀐 값이 전송되도록 합니다.
	*/
	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	uint8 bCanAttack : 1;

	/**
	* bCanAttack 프로퍼티의 OnRep 이벤트 함수입니다.
	* 항상 클라이언트에서만 호출할 수 있습니다.
	* 서버 로직에서는 명시적으로 호출해줘야 합니다.
	*/
	UFUNCTION()
	void OnRep_CanAttack();

	// 공격 모션 시간입니다.
	float AttackTime = 1.1f;
	
	// 마지막에 공격한 시간입니다.
	float LastAttakStartTime = 0.0f;

	/**
	* 클라이언트로부터 패킷을 받은 시간과 서버의 시간은 차이가 존재합니다.	
	*/
	float AttackTimeDifference = 0.0f;

	/**
	* 공격 판정용 변수입니다.
	* 공격자와 피격자 사이의 거리가 3미터 이내면 공격으로, 3미터 초과 시 기각합니다.
	*/
	float AcceptCheckDistance = 500.0f;

	/**
	* 판정 최소 지연 시간
	*/
	float AcceptMinCheckTime = 0.15f;

// PvP section
public:
	void ResetPlayer();
	void ResetAttack();

	FTimerHandle AttackTimerHandle;
	FTimerHandle DeadTimerHandle;

	virtual void SetDead() override;
	virtual void SetStunned() override;
	
    void EndStunned();
    void Knockback(const FVector& Direction);

    FTimerHandle StunnedTimerHandle;
    FVector KnockbackDirection;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;



// Order section
public:
	// void OrderTo(FGOOrder InOrder);


protected:
	/** Use only local*/
	UPROPERTY(VisibleInstanceOnly)
	FGOOrder BufferedOrder;

	/** Use Only Server*/
	UPROPERTY(VisibleInstanceOnly)
	FGOOrder ServerBufferedOrder;


// State section
private:
	UPROPERTY(EditAnywhere, Replicated, Category = "Player", Meta = (Bitmask, BitmaskEnum = "/Script/GuardiansOrders.EGOPlayerActionState"))
	uint32 ActionStateBitMask;

	UPROPERTY(EditDefaultsOnly)
	float DefaultImpactTime = 0.67f;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float ImpactTimer = 0;

	UPROPERTY(EditDefaultsOnly)
	float DefaultBlownRecoveryTime = 1 + 0.43;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float BlownRecoveryTimer = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float InvincibleTimer = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float InvincibleTime = 0;

public:
	void SimulateStateUpdateOnServer(float DeltaTime);

	void SetActionState(EGOPlayerActionState::State State, bool bEnabled);
	void StartState(EGOPlayerActionState::State State, float Duration);
	void EndState(EGOPlayerActionState::State State);
	FTimerHandle StateTimerHandle;

	/**
	 * 현재 Impacted, Cast, Died  상태라면 
	 * (현재 공격을 받고 있거나, 스킬을 사용 중이거나, 죽은 상태라면)
	 */
	uint8 IsOrderExecutableState() const
	{
		return ((ActionStateBitMask & EGOPlayerActionStateValue::OrderUnAcceptableBitMask) == 0);
	}

	bool IsExecutableOrderInOrderNotExecutableState(const FGOOrder& InOrder) const;




	bool IsMoving() const
	{
		return (ActionStateBitMask & EGOPlayerActionState::Move);
	}

	bool IsFlashing() const
	{
		return (ActionStateBitMask & EGOPlayerActionState::Flash);
	}

	bool IsCasting() const
	{
		return (ActionStateBitMask & EGOPlayerActionState::Cast);
	}

	bool IsImpacted() const
	{
		return (ActionStateBitMask & EGOPlayerActionState::Impacted);
	}

	bool IsBlown() const
	{
		return (ActionStateBitMask & EGOPlayerActionState::Blown);
	}

	bool IsInvincible() const
	{
		return (ActionStateBitMask & EGOPlayerActionState::Invincible);
	}

	bool IsDied() const
	{
		return (ActionStateBitMask & EGOPlayerActionState::Died);
	}


	// TObjectPtr<class UAnimMontage> SkillAnimMontage;

	void CheckActorNetworkStatus(AActor* ActorToCheck);

	FName GetHeroTypeFName(EHeroType HeroType)
	{
		switch (HeroType)
		{
		case EHeroType::Rogers: return FName(TEXT("Rogers"));
		case EHeroType::Katniss: return FName(TEXT("Katniss"));
		case EHeroType::Beast: return FName(TEXT("Beast"));
		case EHeroType::Bride: return FName(TEXT("Bride"));
		default: return FName(TEXT("None"));
		}
	}

// Camera Shake
	UPROPERTY(EditAnywhere, Category="Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

// Rotation
public:
	UPROPERTY(ReplicatedUsing = OnRep_Rotation)
	FRotator NetRotation;

	UFUNCTION()
	void OnRep_Rotation();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetRotation(FRotator NewRotation);

private:
	UPROPERTY()
	class AGOBattleGameMode* GOBattleGameMode;

// ======== IGOPlaySkillAnimInterface ========
public:
	virtual UGOSkillCastComponent* GetSkillCastComponent()
	{
		return SkillCastComponent;
	}

	virtual void PlaySkillAnim(UGOSkillBase* CurrentSkill);
	virtual void PlaySkillAnimByKey(FHeroSkillKey Key);
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerCharacter::PlaySkillAnim] 1 called. This function is inherited from GOPlaySkillAnimInterface. "));
	//	GetMesh()->GetAnimInstance()->Montage_Play(CurrentSkill->GetTotalSkillData().SkillAnim);
	//	
	//}

	// 새로 만든: 스킬시스템용 구조체
	virtual void ActivateSkillByKey(FHeroSkillKey Key);

// ======== IGOSpellFlashInterface ========
	virtual void ActivateSpellFlash();

// ======== IGOPlaySkillEffectInterface ========
	virtual void PlayEffectParticleAnimByKey(FHeroSkillKey Key);

// ======== IGOApplySkillInterface ========
	virtual void ApplySkillEffect(AActor* DamagedActor, float Damage, AActor* DamageCauser);

// ======== IGOPlayerInterface ========

	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;

// ======== Move Skill =======
public:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerActivateSkillWithMovement(FHeroSkillKey Key, float Distance, float Duration, float Acceleration);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateSkillWithMovement(FHeroSkillKey Key, float Distance, float Duration, float Acceleration);

	void StartMovingForward(float Distance, float Duration, float Acceleration);

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStunnedAnimation();

public:
	UPROPERTY()
	class AGOPlayerState* GOPlayerState;

	ETeamType GetTeamType();

private:
	// 이동 관련 변수와 타이머 핸들러 추가
	FTimerHandle MovementTimerHandle;
	FVector MovementStartLocation;
	FVector MovementEndLocation;
	float MovementDuration;
	float ElapsedTime;
	float InitialSpeed;
	float CurrentAcceleration;
	float MovementDistance;

	void MoveForwardStep();

protected:
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;


// IGOStateInterface
public:
	virtual bool GetIsDead() override;
	virtual bool GetIsStunned() override;
};
