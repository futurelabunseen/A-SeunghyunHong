// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Share/EGOSkill.h"
#include "GameData/GOSkillDataAsset.h"
#include "GameData/GOSkillStat.h"
#include "GOSkillBase.generated.h"

class UGOSkillStatComponent;
class UAnimMontage;
class UMaterial;
class UNiagaraSystem;

 /*
   * 스킬의 기본적인 동작과 라이프사이클을 정의하며, 스킬 자체의 활성화, 실행, 완료, 중단 등을 관리합니다. 
   */
UCLASS()
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
	bool ReduceCastingTime(float DeltaTime);

	FORCEINLINE const FString& GetName() const { return Name; }
	FORCEINLINE float GetCoolDownTime() const { return GetTotalSkillStat().CoolTime; }
	FORCEINLINE void SetCooldown() { CoolDownTimer = GetTotalSkillStat().CoolTime; }
	// TODO: FORCEINLINE float GetCastingTime() { return GetTotalSkillStat().CastingTime; }
	FORCEINLINE float GetCastingTime() { return CastingTime; }

// Stat Section
public:
	FORCEINLINE void SetCurrentSkillType(const int32 InTypeNumber) { CurrentSkillType = InTypeNumber; }
	FORCEINLINE void SetSkillStat(const FGOSkillStat& InSkillStat) { SkillStat = InSkillStat; }
	FORCEINLINE FGOSkillStat GetTotalSkillStat() const { return SkillStat; }

	void SetSkillStat(int8 InNewSkillType);
	void ResetSkillStat();

private:
	void InitializeSkillStats();

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentSkillType;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = SkillStat)
	FGOSkillStat SkillStat;

// Data Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	UGOSkillDataAsset* SkillDataAsset;

public:
	UPROPERTY(EditDefaultsOnly, Category = SkillSetting)
	FString Name;	
	
	UPROPERTY(EditDefaultsOnly, Category = SkillSetting)
	FText Description;

	UPROPERTY(EditAnywhere, Category = SkillSetting)
	EGOSkillType Type;

	UPROPERTY(EditDefaultsOnly, Category = SkillSetting)
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditDefaultsOnly, Category = SkillSetting)
	TObjectPtr<UAnimMontage> SkillAnim;

	UPROPERTY(EditAnywhere, Category = SkillSetting)
	TObjectPtr <UMaterial> DecalMaterial;

	UPROPERTY(EditAnywhere, Category = SkillSetting)
	TObjectPtr <UNiagaraSystem> VFX;

	// TODO: SFX

	//UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	//float DamageMultiplier = 0;

	//UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	//float DamageRange = 0;
	//
	//UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	//float DamageRadius = 0;
	//
	//UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	//float DamageSpeed = 0;	
	//
	//UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	//float ManaCost = 0;

	//UPROPERTY(EditDefaultsOnly, Replicated, Category = SkillSetting)
	//float CoolTime;

	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float CoolDownTimer = 0;

	//UPROPERTY(EditDefaultsOnly, Category = "SkillSetting/AutoDetection")
	//float DetectionRadius = 0;

	//UPROPERTY(EditDefaultsOnly, Category = "SkillSetting/AutoDetection")
	//float DetectionDegree = 0;

	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float CastingTime = 0;

	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float Id = 0;
};


/**/