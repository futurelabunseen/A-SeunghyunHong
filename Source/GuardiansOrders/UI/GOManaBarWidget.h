// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOUserWidget.h"
#include "GameData/GOCharacterStat.h"
#include "GOManaBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class GUARDIANSORDERS_API UGOManaBarWidget : public UGOUserWidget
{
	GENERATED_BODY()
	
public:
	UGOManaBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	/**
	 * 이 함수가 호출될 때는 UI에 관련된 모든 기능들이 거의 초기화가 완료됐다고 볼 수 있습니다.
	 */
	virtual void NativeConstruct() override;
public:
	void UpdateStat(const FGOCharacterStat& BaseStat, const FGOCharacterStat& ModifierStat);
	// FORCEINLINE void SetMaxMana(float NewMaxMana) { MaxMana = NewMaxMana; }
	void UpdateManaBar(float NewCurrentMana, float NewMaxMana);
	FString GetManaStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> ManaProgressBar;

	UPROPERTY()
	TObjectPtr<UTextBlock> ManaStat;

	UPROPERTY()
	float CurrentMana;

	UPROPERTY()
	float MaxMana;

};
