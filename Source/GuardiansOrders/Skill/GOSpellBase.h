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
	// ��ų �����͸� �����ϴ� �Լ��Դϴ�.
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
	float CoolDownTimer = 0.0f; // SetCoolDownTime() ��������!!!
	bool bIsOnCoolTime = false;

public:
	/**
	  * ��ų�� ���۵� �� ȣ��Ǵ� �Լ���, ��ų ����� �ʱ�ȭ �ܰ踦 ó���մϴ�.
	  */
	virtual void StartCast();

	/**
	  * ��ų�� Ȱ��ȭ�Ǵ� ���� ���������� ȣ��Ǹ�, �̴� �ַ� ��ų�� ���� ���¸� �����ϰų�, �ð��� ���� ��ȭ�ϴ� ȿ���� �����ϴ� �� ���˴ϴ�.
	  */
	virtual void UpdateCast(float DeltaTime);

	/**
	  * ��ü���� ��ų ��������, UpdateCast ������ ȣ����� ������- ��� ���� ��
	  * ��ų�� ����, ����̰ų� �ð� ���� �ΰ� ������ �� �� �����Ƿ�?
	  */
	virtual void Activate();

	/**
	  * ��ų ����� �Ϸ���� �� ȣ��Ǵ� �Լ���, ��ų�� ����� ó���ϰ� �������ϴ� �۾��� �����մϴ�.
	  */
	virtual void FinishCast();

	/**
	  * ��ų�� �ߴܵ� �� ȣ��Ǵ� �Լ���, ���� ��� ĳ���Ͱ� �ǰݴ����� �� ��ų ����� �ߴܽ�Ű�� �ʿ��� ���� �۾��� �����մϴ�.
	  */
	virtual void InterruptedCast();

	/**
	  * ��ų�� VFX ȿ���� ����մϴ�.
	  */
	virtual void ActivateEffect();

	bool bIsCasting = false;
	bool bIsCastable = true;

public:
	virtual void Tick(float DeltaTime) override;
	/** Tick�� Ȱ��ȭ���� ���θ� ��ȯ�ϴ� �Լ��Դϴ�. */
	virtual bool IsTickable() const override;
	/** �����Ϳ��� Tick�� Ȱ��ȭ���� ���θ� ��ȯ�ϴ� �Լ��Դϴ�. �Ϲ������� true�� ��ȯ�Ͽ� �����Ϳ����� ��ü�� Tick�� Ȱ��ȭ�� �� �ֵ��� �����մϴ�. */
	virtual bool IsTickableInEditor() const override;
	/** ������ �Ͻ� �����Ǿ��� �� Tick�� ����ؼ� ȣ������ ���θ� ��ȯ�ϴ� �Լ��Դϴ�. */
	virtual bool IsTickableWhenPaused() const override;
	/** ��ü�� ������ ��� �ĺ��ڸ� ��ȯ�ϴ� �Լ��Դϴ�. */
	virtual TStatId GetStatId() const override;
	/** ��ü�� �����ϴ� World�� ��ȯ�ϴ� �Լ��Դϴ�. */
	virtual UWorld* GetWorld() const override;

protected:
	/** Tick �Լ��� Ȱ��ȭ/��Ȱ��ȭ�� �����ϴ� �����Դϴ�. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseSkill")
	bool bTickable;

	/** ������ �Ͻ� �����Ǿ��� �� Tick �Լ��� ȣ������ ��Ÿ���� �����Դϴ�. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseSkill")
	bool bTickableWhenPaused;
};
