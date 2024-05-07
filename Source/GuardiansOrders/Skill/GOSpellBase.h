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
	// 스킬 데이터를 설정하는 함수입니다.
	UFUNCTION(BlueprintCallable, Category = "Data set")
	void InitializeSpell(FName InSpellName);

public:
	FORCEINLINE FGOSpellStat GetTotalSpellStat() const { return SpellStat; }
	FORCEINLINE FGOSpellData GetTotalSpellData() const { return SpellData; }
	FORCEINLINE float GetCastingTime() { return GetTotalSpellStat().CastingTime; }

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
};
