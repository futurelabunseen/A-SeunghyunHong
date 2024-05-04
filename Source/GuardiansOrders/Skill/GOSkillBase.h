// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Share/EGOSkill.h"
#include "GameData/GOSkillDataAsset.h"
#include "GameData/GOSkillStat.h"
#include "GameData/GOSkillData.h"
#include "Delegates/Delegate.h"
#include "GOSkillBase.generated.h"

class UGOSkillStatComponent;
class UAnimMontage;
class UMaterial;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCooldownUpdated, float);

 /*
   * 스킬의 기본적인 동작과 라이프사이클을 정의하며, 스킬 자체의 활성화, 실행, 완료, 중단 등을 관리합니다. 
   */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOSkillBase : public UObject
{
	GENERATED_BODY()

public:
	UGOSkillBase();
	FOnCooldownUpdated OnCooldownUpdated;
	virtual void PostInitProperties() override;
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

	void SetSkillOwner(AActor* NewOwner);
	FORCEINLINE AActor* GetSkillOwner() { return SkillOwnerCharacter; };

	bool IsCastable() const { return bIsCastable; };
	bool IsCasting() const { return bIsCasting; };
	//virtual bool IsCastable() const { return CoolDownTimer <= 0; }
	//bool IsCasting() const { return bIsOnCasting; }

public:
	/**
	  * 스킬이 시작될 때 호출되는 함수로, 스킬 사용의 초기화 단계를 처리합니다.
	  */
	virtual void StartCast();

	/**
	  * 스킬이 활성화되는 동안 지속적으로 호출되며, 이는 주로 스킬의 진행 상태를 갱신하거나, 시간에 따라 변화하는 효과를 관리하는 데 사용됩니다.
	  */
	virtual void UpdateCast(float DeltaTime);

	/**
	  * 구체적인 스킬 내용으로, UpdateCast 내에서 호출되지 않을까- 라고 생각 중
	  * 스킬에 따라서, 즉발이거나 시간 차를 두고 공격이 될 수 있으므로?
	  */
	virtual void Activate();

	/**
	  * 스킬 사용이 완료됐을 때 호출되는 함수로, 스킬의 결과를 처리하고 마무리하는 작업을 수행합니다.
	  */
	virtual void FinishCast();

	/**
	  * 스킬이 중단될 때 호출되는 함수로, 예를 들어 캐릭터가 피격당했을 때 스킬 사용을 중단시키고 필요한 정리 작업을 수행합니다.
	  */
	virtual void InterruptedCast();

	/**
	  * 스킬의 VFX 효과를 재생합니다.
	  */
	virtual void ActivateEffect();

// Stat & Data Section
public:
	FORCEINLINE void SetSkillStat(const FGOSkillStat& InSkillStat) { SkillStat = InSkillStat; }
	FORCEINLINE FGOSkillStat GetTotalSkillStat() const { return SkillStat; }
	FORCEINLINE FGOSkillData GetTotalSkillData() const { return SkillData; }
	//void ResetSkillStat();

	FORCEINLINE float GetCoolDownTime() const { return GetTotalSkillStat().CoolDownTime; }
	FORCEINLINE void SetCoolDownTimer() { CoolDownTimer = GetTotalSkillStat().CoolDownTime; }
	FORCEINLINE float GetCoolDownTimer() const { return CoolDownTimer; }
	FORCEINLINE float GetCastingTime() { return GetTotalSkillStat().CastingTime; }

	FORCEINLINE ESkillTriggerType GetSkillTriggerType() const { return GetTotalSkillData().SkillTriggerType; }
	FORCEINLINE ESkillAffectType GetSkillAffectType() const { return GetTotalSkillData().SkillAffectType; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	FGOSkillStat SkillStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	FGOSkillData SkillData;

	// 스킬 소유자에 대한 참조입니다.
	TObjectPtr<AActor> SkillOwnerCharacter;
	// 스킬의 피격자가 될 캐릭터에 대한 참조입니다.
	TObjectPtr<AActor> SkillTargetCharacter;

	// Data Section
public:
	// 스킬 데이터를 설정하는 함수입니다.
	UFUNCTION(BlueprintCallable, Category = "Data set")
	void InitializeSkill(FName InSkillName);

public:
	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float CoolDownTimer = 0; // SetCoolDownTime() 잊지말기!!!

public:
	// 스킬 시전 가능한지의 여부
	bool bIsCastable = true;

	// 스킬이 현재 캐스팅 중인지의 여부
	bool bIsCasting = false;

};