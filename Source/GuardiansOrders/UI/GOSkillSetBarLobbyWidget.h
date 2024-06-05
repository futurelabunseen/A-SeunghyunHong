// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GOSkillSetBarLobbyWidget.generated.h"

class UGOSkillSlotLobbyWidget;
/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOSkillSetBarLobbyWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UGOSkillSetBarLobbyWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGOSkillSlotLobbyWidget> SkillSlot_LMB_Widget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGOSkillSlotLobbyWidget> SkillSlot_Q_Widget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGOSkillSlotLobbyWidget> SkillSlot_W_Widget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGOSkillSlotLobbyWidget> SkillSlot_E_Widget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGOSkillSlotLobbyWidget> SkillSlot_R_Widget;
};
