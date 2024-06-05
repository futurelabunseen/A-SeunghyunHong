// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOSkillSetBarLobbyWidget.h"
#include "UI/GOSkillSlotLobbyWidget.h"

UGOSkillSetBarLobbyWidget::UGOSkillSetBarLobbyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGOSkillSetBarLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillSlot_LMB_Widget = Cast<UGOSkillSlotLobbyWidget>(GetWidgetFromName(TEXT("SkillSlot_LMB_Widget")));
	SkillSlot_Q_Widget = Cast<UGOSkillSlotLobbyWidget>(GetWidgetFromName(TEXT("SkillSlot_Q_Widget")));
	SkillSlot_W_Widget = Cast<UGOSkillSlotLobbyWidget>(GetWidgetFromName(TEXT("SkillSlot_W_Widget")));
	SkillSlot_E_Widget = Cast<UGOSkillSlotLobbyWidget>(GetWidgetFromName(TEXT("SkillSlot_E_Widget")));
	SkillSlot_R_Widget = Cast<UGOSkillSlotLobbyWidget>(GetWidgetFromName(TEXT("SkillSlot_R_Widget")));
}
