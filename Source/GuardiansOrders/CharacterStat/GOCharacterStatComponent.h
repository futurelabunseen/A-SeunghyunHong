// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/GOCharacterStat.h"
#include "GOCharacterStatComponent.generated.h"

// Hp Delegate
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float /*CurrentHp*/, float /*MaxHp*/);

// Mana Delegate
DECLARE_MULTICAST_DELEGATE(FOnManaZeroDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnManaChangedDelegate, float /*CurrnetMana*/, float /*MaxMana*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FGOCharacterStat& /*BaseStat*/, const FGOCharacterStat& /*ModifierStat*/);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStatIncreased, float OldStat, float IncreaseAmount, float NewStat);

// For Skill Slot Widget
DECLARE_MULTICAST_DELEGATE_OneParam(FUGOCharacterStatComponentOnManaChangedDelegate, float /*CurrnetMana*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUARDIANSORDERS_API UGOCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGOCharacterStatComponent();
	FUGOCharacterStatComponentOnManaChangedDelegate UGOCharacterStatComponentOnManaChangedDelegate;

protected:
	virtual void InitializeComponent() override;

public:
// Hp Delegate
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

// Mana Delegate
	FOnManaZeroDelegate OnManaZero;
	FOnManaChangedDelegate OnManaChanged;

	FOnStatChangedDelegate OnStatChanged;

	FOnStatIncreased OnStatIncreased;
	//void SetCharacterStat(int8 InNewCharacterType);
	void SetCharacterStat(FName InCharacterName);
	FORCEINLINE void SetCurrentCharacterType(const float InTypeNumber) { CurrentCharacterType = InTypeNumber; }
	FORCEINLINE float GetCurrentCharacterType() const { return CurrentCharacterType; }
	
	FORCEINLINE void SetBaseStat(const FGOCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetModifierStat(const FGOCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	
	FORCEINLINE const FGOCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FGOCharacterStat& GetModifierStat() const { return ModifierStat; }

	//FORCEINLINE FGOCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetMaxHp() { return MaxHp; }

	void HealHp(float HealAmount);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHealHp(float HealHpAmount);

	// 내 캐릭터가 대미지를 받으면 적용하는 함수입니다.
	float ApplyDamage(float InDamage);

	FORCEINLINE float GetMaxMana() { return MaxMana; }
	FORCEINLINE float GetCurrentMana() { return CurrentMana; }
	// 내 캐릭터가 스킬을 사용하면 적용하는 함수입니다.
	float UseSkill(float SkillManaCost);
	void SetMana(float NewMana);

	FORCEINLINE float GetMaxBasicAttackRange() const { return BaseStat.MaxBasicAttackRange; }

	// Function to get total stat
	FGOCharacterStat GetTotalStat() const;

	// Function to increase base damage
	void IncreaseBaseDamage(float Amount);

	// Function to log stat changes
	void LogStatChange(const FString& Context);

protected:
	// called when hp is changed
	void SetHp(float NewHp);

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHp, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHp, Transient, VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentCharacterType;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_BaseStat, VisibleInstanceOnly, Category = Stat)
	FGOCharacterStat BaseStat;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_ModifierStat, VisibleInstanceOnly, Category = Stat)
	FGOCharacterStat ModifierStat;

// Mana Section
protected:
	UPROPERTY(ReplicatedUsing = OnRep_MaxMana,Transient, VisibleInstanceOnly, Category = Stat)
	float MaxMana;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentMana, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentMana;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stat)
	float HpRegenerationRate;

	FTimerHandle HpRegenerationTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stat)
	float ManaRegenerationRate;

	FTimerHandle ManaRegenerationTimer;

protected:
	virtual void BeginPlay() override;
	virtual void ReadyForReplication() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetNewMaxHp(const FGOCharacterStat& InBaseStat, const FGOCharacterStat& InModifierStat);
	void SetNewMaxMana(const FGOCharacterStat& InBaseStat, const FGOCharacterStat& InModifierStat);
	void RegenerateHp();
	void RegenerateMana();

	UFUNCTION()
	void OnRep_CurrentHp();

	UFUNCTION()
	void OnRep_MaxHp();

	UFUNCTION()
	void OnRep_CurrentMana();

	UFUNCTION()
	void OnRep_MaxMana();

	UFUNCTION()
	void OnRep_BaseStat();

	UFUNCTION()
	void OnRep_ModifierStat();

public:
	void ResetStat();
};
