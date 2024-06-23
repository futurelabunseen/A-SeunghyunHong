// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOElimAnnouncementWidget.h"
#include "CommonTextBlock.h"

void UGOElimAnnouncementWidget::SetElimAnnouncementText(FString AttackerName, FString VictimName)
{
	FString ElimAnnouncementText = FString::Printf(TEXT("%s killed %s"), *AttackerName, *VictimName);
	if (AnnouncementText)
	{
		AnnouncementText->SetText(FText::FromString(ElimAnnouncementText));
	}
}
