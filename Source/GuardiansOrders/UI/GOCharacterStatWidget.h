// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameData/GOCharacterStat.h"
#include "GOCharacterStatWidget.generated.h"

class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOCharacterStatWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FGOCharacterStat& BaseStat, const FGOCharacterStat& ModifierStat);

private:
	UPROPERTY()
	TMap<FName, UCommonTextBlock*> BaseLookup;

	UPROPERTY()
	TMap<FName, UCommonTextBlock*> ModifierLookup;
};
