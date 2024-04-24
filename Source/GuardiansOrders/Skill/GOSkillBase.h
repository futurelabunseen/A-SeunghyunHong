// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Share/EGOSkill.h"
#include "GameData/GOSkillDataAsset.h"
#include "GameData/GOSkillStat.h"

#include "GameData/GOSkillData.h"

#include "GOSkillBase.generated.h"

class UGOSkillStatComponent;
class UAnimMontage;
class UMaterial;
class UNiagaraSystem;

 /*
   * 스킬의 기본적인 동작과 라이프사이클을 정의하며, 스킬 자체의 활성화, 실행, 완료, 중단 등을 관리합니다. 
   */
UCLASS(BlueprintType)
class GUARDIANSORDERS_API UGOSkillBase : public UObject
{
	GENERATED_BODY()

public:
	UGOSkillBase();
	virtual void PostInitProperties() override;
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

public:
	/**
	  * 구체적인 스킬 내용
      */
	virtual void Activate() {};
	
	/**
	  * 스킬이 시작될 때 호출되는 함수로, 스킬 사용의 초기화 단계를 처리합니다.
	  */
	virtual void StartCast() {};
	
	/**
	  * 스킬이 활성화되는 동안 지속적으로 호출되며, 이는 주로 스킬의 진행 상태를 갱신하거나, 시간에 따라 변화하는 효과를 관리하는 데 사용됩니다.
	  */
	virtual void UpdateCast(float DeltaTime) {};
	
	/**
	  * 스킬 사용이 완료됐을 때 호출되는 함수로, 스킬의 결과를 처리하고 마무리하는 작업을 수행합니다.
	  */
	virtual void FinishCast() {};
	
	/**
	  * 스킬이 중단될 때 호출되는 함수로, 예를 들어 캐릭터가 피격당했을 때 스킬 사용을 중단시키고 필요한 정리 작업을 수행합니다.
	  */
	virtual void InterruptedCast() {};

	/**
	  * 스킬의 VFX 효과를 재생합니다.
	  */
	virtual void ActivateEffect() {};

public:

// Stat & Data Section
public:
	FORCEINLINE void SetSkillStat(const FGOSkillStat& InSkillStat) { SkillStat = InSkillStat; }
	FORCEINLINE FGOSkillStat GetTotalSkillStat() const { return SkillStat; }
	FORCEINLINE FGOSkillData GetTotalSkillData() const { return SkillData; }
	//void ResetSkillStat();

	bool ReduceCastingTime(float DeltaTime);
	FORCEINLINE float GetCoolDownTime() const { return GetTotalSkillStat().CoolDownTime; }
	FORCEINLINE void SetCoolDownTime() { CoolDownTimer = GetTotalSkillStat().CoolDownTime; }
	FORCEINLINE float GetCastingTime() { return GetTotalSkillStat().CastingTime; }

	FORCEINLINE ESkillTriggerType GetSkillTriggerType() const { return GetTotalSkillData().SkillTriggerType; }
	FORCEINLINE ESkillAffectType GetSkillAffectType() const { return GetTotalSkillData().SkillAffectType; }

protected:

	UPROPERTY(Transient, VisibleInstanceOnly, Category = SkillStat)
	FGOSkillStat SkillStat;

	UPROPERTY(VisibleInstanceOnly, Category = SkillData)
	FGOSkillData SkillData;

// Data Section
public:
	// 스킬 데이터를 설정하는 함수입니다.
	UFUNCTION(BlueprintCallable, Category = "Data set")
	void Set(FName InSkillName);

	// 스킬 데이터 테이블 참조입니다.
	UPROPERTY(EditDefaultsOnly, Category = "Skill Data")
	UDataTable* SkillDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Skill Data")
	UDataTable* SkillStatDataTable;
	
public:
	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float CoolDownTimer = 0;
};