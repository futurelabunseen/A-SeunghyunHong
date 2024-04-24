// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/GOAnimationAttackInterface.h"
#include "Interface/GOCharacterWidgetInterface.h"
#include "Interface/GOPlaySkillAnimInterface.h"

#include "GameData/GOCharacterStat.h"
#include "GameData/GOCharacterData.h"
#include "Skill/GOSkillBase.h"
#include "GOCharacterBase.generated.h"

class UGOCharacterStatComponent;
class UGOWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
class UGOUserWidget;
class UGOSkillCastComponent;

UCLASS()
class GUARDIANSORDERS_API AGOCharacterBase : public ACharacter, public IGOAnimationAttackInterface, public IGOCharacterWidgetInterface, public IGOPlaySkillAnimInterface
{
	GENERATED_BODY()
	
public:
	AGOCharacterBase();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

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

// Skill Slot Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillSlot)
	TArray<UGOSkillBase*> SkillSlot;

	const int32 SKILLSOT_CAPACITY{ 5 };

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

// ======== IPlaySkillAnimInterface ========

	virtual UGOSkillCastComponent* GetSkillCastComponent()
	{
		return SkillCastComponent;
	}

	virtual void PlaySkillAnim(UGOSkillBase* CurrentSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACharacterBase::PlaySkillAnim] called. This function is inherited from GOPlaySkillAnimInterface. "));
	}

}; // End Of Class
