// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOStatsBarWidget.h"
#include "Interface/GOCharacterWidgetInterface.h"

UGOStatsBarWidget::UGOStatsBarWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UGOStatsBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IGOCharacterWidgetInterface* CharacterWidget = Cast<IGOCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}
