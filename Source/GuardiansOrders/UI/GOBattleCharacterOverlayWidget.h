// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GOBattleCharacterOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOBattleCharacterOverlayWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UCommonTextBlock* ScoreAmount;	
	
	UPROPERTY(meta=(BindWidget))
	class UCommonTextBlock* DefeatsAmount; 	

	UPROPERTY(meta=(BindWidget))
	class UCommonTextBlock* MatchCountdownText;
};
