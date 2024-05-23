// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOLobbyHUDWidget.h"
#include "UI/GOHeroSelectionWidget.h"
#include "Interface/GOLobbyHUDInterface.h"
#include "CommonTextBlock.h"

UGOLobbyHUDWidget::UGOLobbyHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGOLobbyHUDWidget::InitializeHeroSelection()
{
	if (HeroSelectionWidget)
	{
		// 여기서 영웅 선택 위젯을 초기화
	}
}

void UGOLobbyHUDWidget::ShowHeroSelectionWidget()
{
	if (HeroSelectionWidget)
	{
		HeroSelectionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGOLobbyHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HeroSelectionWidget = Cast<UGOHeroSelectionWidget>(GetWidgetFromName(TEXT("WidgetHeroSelection")));
	ensure(HeroSelectionWidget);
	HeroSelectionWidget->SetVisibility(ESlateVisibility::Hidden);

	CountdownText = Cast<UCommonTextBlock>(GetWidgetFromName(TEXT("CountdownText")));
	ensure(CountdownText);
	CountdownText->SetVisibility(ESlateVisibility::Hidden);

	IGOLobbyHUDInterface* HUDController = Cast<IGOLobbyHUDInterface>(GetOwningPlayer());
	if (HUDController)
	{
		HUDController->SetupLobbyHUDWidget(this);
	}
}
