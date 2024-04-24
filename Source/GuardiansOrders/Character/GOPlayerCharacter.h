// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GOCharacterBase.h"
#include "Interface/GOCharacterHUDInterface.h"
#include "Share/ShareEnums.h" 
#include "GameData/GOCharacterDataAsset.h"
#include "GameData/GOCharacterStat.h"
#include "Share/EGOPlayerActionState.h"
#include "Share/GOOrder.h"
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
class GUARDIANSORDERS_API AGOPlayerCharacter : public AGOCharacterBase, public IGOCharacterHUDInterface
{
	GENERATED_BODY()
	
public:
	AGOPlayerCharacter();
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
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero Data")
	UGOCharacterDataAsset* HeroDataAsset;

	EHeroType HeroType;
	ERoleType RoleType;
	EAttackRange AttackRange;
	EArchetype Archetype;

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
	TObjectPtr<UInputAction> ActionSkillF;


	void MoveGamePad(const FInputActionValue& Value);

	// Input handlers for SetDestination action
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	virtual void OnSkillQ();
	virtual void OnSkillW();
	virtual void OnSkillE();
	virtual void OnSkillR();
	void OnSkillF();

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
	* 공격을 판정하는 함수입니다.
	*/
	virtual void AttackHitCheck() override;

	/**
	* 공격을 확정합니다.
	*/
	void AttackHitConfirm(AActor* HitActor);

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

	/**
	* 
	*/
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAttack();

	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayAnimation(AGOPlayerCharacter* CharacterToPlay);
	
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
	float AcceptCheckDistance = 300.0f;

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
	UPROPERTY(Replicated, VisibleInstanceOnly,
	Meta = (Bitmask, BitmaskEnum = "EGOPlayerActionState"), Category = "Player State")
	uint32 ActionStateBitMask = 0;

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

};
