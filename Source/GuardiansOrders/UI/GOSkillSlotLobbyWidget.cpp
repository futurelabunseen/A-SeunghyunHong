// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOSkillSlotLobbyWidget.h"
#include "UI/GOSkillSlotToolTipWidget.h"
#include "UI/SkillWidget/GOSkillSlotWidget.h"

void UGOSkillSlotLobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ToolTipClass)
	{

		GOToolTipWidget = CreateWidget<UGOSkillSlotToolTipWidget>(this, ToolTipClass);
		GOToolTipWidget->LobbySkillSlotBeingHobered = this;
		GOToolTipWidget->BattleSkillSlotBeingHobered = nullptr;

		SetToolTip(GOToolTipWidget);
		UE_LOG(LogTemp, Warning, TEXT("[ToolTip] skill lobby"));
	}
}

void UGOSkillSlotLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
