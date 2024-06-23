// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GOSkillSlotToolTipWidget.generated.h"
class UCommonTextBlock;
class UGOSkillSlotLobbyWidget;
class UGOSkillSlotWidget;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOSkillSlotToolTipWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UGOSkillSlotToolTipWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGOSkillSlotLobbyWidget> LobbySkillSlotBeingHobered;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGOSkillSlotWidget> BattleSkillSlotBeingHobered;

	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UGOSkillSlotWidget> SkillSlotBeingHobered;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* SkillNameText;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* SkillDescriptionText;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* SkillTriggerTypeText;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* SkillAffectTypeText;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* SkillCastTypeText;	

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CoolTimeText;	

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ManaCostText;

	void UpdateSkillToolTip(FName SkillName);
	FString GetEnumValuesAsString(const FString& EnumName, int32 EnumValue);

protected:
	virtual void NativeConstruct() override;

};
