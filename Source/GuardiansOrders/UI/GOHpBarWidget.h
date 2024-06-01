// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOUserWidget.h"
#include "GameData/GOCharacterStat.h"
#include "GOHpBarWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;

UCLASS()
class GUARDIANSORDERS_API UGOHpBarWidget : public UGOUserWidget
{
	GENERATED_BODY()
	
public:
	UGOHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHpBar(float NewCurrentHp, float MaxHp);
	FString GetHpStatText();
	void SetHpBarTexture(UTexture2D* NewTexture);  // Setting the HP bar texture

protected:
	UPROPERTY()
	TObjectPtr<UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;
	
	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	TObjectPtr<UImage> HpBarImage;  // Image for the progress bar

};
