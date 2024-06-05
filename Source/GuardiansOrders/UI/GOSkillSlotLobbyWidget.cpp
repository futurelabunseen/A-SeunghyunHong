// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOSkillSlotLobbyWidget.h"
#include "UI/GOSkillSlotToolTipWidget.h"

void UGOSkillSlotLobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ToolTipClass)
	{

		GOToolTipWidget = CreateWidget<UGOSkillSlotToolTipWidget>(this, ToolTipClass);
		GOToolTipWidget->LobbySkillSlotBeingHobered = this;
		SetToolTip(GOToolTipWidget);
		UE_LOG(LogTemp, Warning, TEXT("[ToolTip]"));
	}
}

void UGOSkillSlotLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
