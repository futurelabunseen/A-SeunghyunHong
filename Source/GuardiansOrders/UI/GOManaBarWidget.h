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
	 * �� �Լ��� ȣ��� ���� UI�� ���õ� ��� ��ɵ��� ���� �ʱ�ȭ�� �Ϸ�ƴٰ� �� �� �ֽ��ϴ�.
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
