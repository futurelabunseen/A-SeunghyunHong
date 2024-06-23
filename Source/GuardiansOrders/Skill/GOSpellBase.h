// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/GOSpellStat.h"
#include "GameData/GOSpellData.h"
#include "Tickable.h"
#include "GOSpellBase.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(UGOSpellBaseFIsOnCooldown, bool);

UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOSpellBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UGOSpellBase();

	UGOSpellBaseFIsOnCooldown UGOSpellBaseFIsOnCooldown;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spells")
	FGOSpellStat SpellStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spells")
	FGOSpellData SpellData;

public:
	// 스킬 데이터를 설정하는 함수입니다.
	UFUNCTION(BlueprintCallable, Category = "Data set")
	void InitializeSpell(FName InSpellName);

public:
	FORCEINLINE FGOSpellStat GetTotalSpellStat() const { return SpellStat; }
	FORCEINLINE FGOSpellData GetTotalSpellData() const { return SpellData; }
	FORCEINLINE float GetCastingTime() { return GetTotalSpellStat().CastingTime; }
	FORCEINLINE float GetCoolDownTime() const { return GetTotalSpellStat().CoolDownTime; }
	FORCEINLINE void SetCoolDownTimer() { CoolDownTimer = GetTotalSpellStat().CoolDownTime; }
	FORCEINLINE float GetCoolDownTimer() const { return CoolDownTimer; }
	FORCEINLINE bool GetIsOnCoolTime() { return bIsOnCoolTime; }
	FORCEINLINE void SetIsOnCoolTime(bool Inbool) { bIsOnCoolTime = Inbool; }
	float CoolDownTimer = 0.0f; // SetCoolDownTime() 잊지말기!!!
	bool bIsOnCoolTime = false;

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

	bool bIsCasting = false;
	bool bIsCastable = true;

public:
	virtual void Tick(float DeltaTime) override;
	/** Tick을 활성화할지 여부를 반환하는 함수입니다. */
	virtual bool IsTickable() const override;
	/** 에디터에서 Tick을 활성화할지 여부를 반환하는 함수입니다. 일반적으로 true를 반환하여 에디터에서도 객체의 Tick을 활성화할 수 있도록 설정합니다. */
	virtual bool IsTickableInEditor() const override;
	/** 게임이 일시 정지되었을 때 Tick을 계속해서 호출할지 여부를 반환하는 함수입니다. */
	virtual bool IsTickableWhenPaused() const override;
	/** 객체의 고유한 통계 식별자를 반환하는 함수입니다. */
	virtual TStatId GetStatId() const override;
	/** 객체가 존재하는 World를 반환하는 함수입니다. */
	virtual UWorld* GetWorld() const override;

protected:
	/** Tick 함수의 활성화/비활성화를 설정하는 변수입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseSkill")
	bool bTickable;

	/** 게임이 일시 정지되었을 때 Tick 함수를 호출할지 나타내는 변수입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseSkill")
	bool bTickableWhenPaused;
};
