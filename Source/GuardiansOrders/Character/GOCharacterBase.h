// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/GOAnimationAttackInterface.h"
#include "Interface/GOCharacterWidgetInterface.h"
#include "GameData/GOCharacterStat.h"
#include "GameData/GOCharacterData.h"
#include "Skill/GOSkillBase.h"
#include "Share/EGOPlayerActionState.h"
#include "Share/GOOrder.h"
#include "GOCharacterBase.generated.h"

class UGOCharacterStatComponent;
class UGOWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
class UGOUserWidget;
class UGOSkillCastComponent;

UCLASS()
class GUARDIANSORDERS_API AGOCharacterBase : public ACharacter, public IGOAnimationAttackInterface, public IGOCharacterWidgetInterface
{
	GENERATED_BODY()
	
public:
	AGOCharacterBase();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

// Data Section
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetData(FName InCharacterName);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetCharacterStatData(FName InCharacterName);

	// 개별 스킬 데이터를 설정합니다.
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void SetSkillDataQ(FName InSkillName);

	UFUNCTION(BlueprintCallable, Category = "Skills")
	void SetSkillDataW(FName InSkillName);

	UFUNCTION(BlueprintCallable, Category = "Skills")
	void SetSkillDataE(FName InSkillName);

	UFUNCTION(BlueprintCallable, Category = "Skills")
	void SetSkillDataR(FName InSkillName);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* CharacterDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* CharacterStatDataTable;

	FGOCharacterData CharacterData;

	FGOCharacterStat CharacterStat;

	// 스킬 클래스 정보
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	TSubclassOf<UGOSkillBase> SkillQClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	TSubclassOf<UGOSkillBase> SkillWClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	TSubclassOf<UGOSkillBase> SkillEClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	TSubclassOf<UGOSkillBase> SkillRClass;

	// 스킬 인스턴스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	TObjectPtr<UGOSkillBase> SkillQInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	TObjectPtr<UGOSkillBase> SkillWInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	TObjectPtr<UGOSkillBase> SkillEInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	TObjectPtr<UGOSkillBase> SkillRInstance;

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess="true"))
	TObjectPtr<UGOCharacterStatComponent> Stat;

	void ApplyStat(const FGOCharacterStat& BaseStat, const FGOCharacterStat& ModifierStat);

// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGOWidgetComponent> HpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGOWidgetComponent> ManaBar;

	// HpBar, ManaBar 따로 관리하는 대신 이렇게 통합해서 만들기?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGOWidgetComponent> StatsBar;

	virtual void SetupCharacterWidget(UGOUserWidget* InUserWidget) override;

	// virtual void SetupHUDWidget(UGOHUDWidget* InHUDWidget) override;

// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// Skill Action Section
protected:
	UGOSkillCastComponent* SkillCastComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	void ProcessComboCommand();

// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

// Mana Section
protected:
	// 마나가 충전되는 함수입니다.
	virtual void GetMana();

	// 마나가 없을 때 호출되는 함수입니다.
	virtual void NoMana();

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
	 * IsOrderExecutableState: 현재 Impacted, Cast, Died 상태라면 명령 처리 불가
	 * (현재 공격을 받고 있거나, 스킬을 사용 중이거나, 죽은 상태라면)
	 * TODO: HasEnoughMana?
	 */
	uint8 IsOrderExecutableState() const
	{
		return ((ActionStateBitMask & EGOPlayerActionStateValue::OrderUnAcceptableBitMask) == 0);
	}

	bool IsExecutableOrderInOrderNotExecutableState(const FGOOrder& InOrder) const;

    // 상태 설정, 제거, 확인
    void SetCharacterActionState(EGOPlayerActionState::State State);
    void ClearCharacterActionState(EGOPlayerActionState::State State);
    bool IsCharacterActionStateSet(EGOPlayerActionState::State State) const;

	bool IsMoving() const
	{
		return IsCharacterActionStateSet(EGOPlayerActionState::Move);
		// return (ActionStateBitMask & EGOPlayerActionState::Move);
	}

	bool IsFlashing() const
	{
		return IsCharacterActionStateSet(EGOPlayerActionState::Flash);
		// return (ActionStateBitMask & EGOPlayerActionState::Flash);
	}

	bool IsCasting() const
	{
		return IsCharacterActionStateSet(EGOPlayerActionState::Cast);
		// return (ActionStateBitMask & EGOPlayerActionState::Cast);
	}

	bool IsImpacted() const
	{
		return IsCharacterActionStateSet(EGOPlayerActionState::Impacted);
		// return (ActionStateBitMask & EGOPlayerActionState::Impacted);
	}

	bool IsBlown() const
	{
		return IsCharacterActionStateSet(EGOPlayerActionState::Blown);
		// return (ActionStateBitMask & EGOPlayerActionState::Blown);
	}

	bool IsInvincible() const
	{
		return IsCharacterActionStateSet(EGOPlayerActionState::Invincible);
		// return (ActionStateBitMask & EGOPlayerActionState::Invincible);
	}

	bool IsDead() const
	{
		return IsCharacterActionStateSet(EGOPlayerActionState::Died);
		// return (ActionStateBitMask & EGOPlayerActionState::Died);
	}

//#if defined(UE_BUILD_DEBUG) || defined(UE_BUILD_DEVELOPMENT)
//
//	private
//		:
//			void PrintCharacterStateOnScreen() const
//			{
//				GEngine->AddOnScreenDebugMessage(11, 10.f, FColor::Green,
//					FString::Printf(TEXT(
//						"[CharacterActionState]\n"
//						"IsMoving: %d\nIsFlashing: %d\nIsAttacking: %d\n"
//						"IsImpacted: %d\nIsBlown: %d\nIsInvincible: %d\nIsDead: %d\n"\
//					),
//						IsMoving(), 
//						IsFlashing(),
//						IsCasting(),
//						IsImpacted(),
//						IsBlown(),
//						IsInvincible(),
//						IsDead()
//					));
//			}
//
//#endif
}; // End Of Class
