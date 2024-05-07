// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/GOSpellStat.h"
#include "GameData/GOSpellData.h"
#include "GOSpellBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOSpellBase : public UObject
{
	GENERATED_BODY()
	
public:
	UGOSpellBase();

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
};
