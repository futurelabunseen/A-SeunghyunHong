// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GOBattleCharacterOverlayWidget.generated.h"

class UGOTeamMemberWidget;
class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOBattleCharacterOverlayWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ScoreAmount;	
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* DefeatsAmount; 	

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* MatchCountdownText;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* BlueTeamScore;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* RedTeamScore;

	UPROPERTY(meta=(BindWidget))
	UGOTeamMemberWidget* BlueTeamMember01;

	UPROPERTY(meta = (BindWidget))
	UGOTeamMemberWidget* BlueTeamMember02;

	UPROPERTY(meta = (BindWidget))
	UGOTeamMemberWidget* RedTeamMember01;

	UPROPERTY(meta = (BindWidget))
	UGOTeamMemberWidget* RedTeamMember02;
};
