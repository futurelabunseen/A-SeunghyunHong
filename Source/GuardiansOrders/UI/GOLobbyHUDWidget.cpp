// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOLobbyHUDWidget.h"
#include "UI/GOHeroSelectionWidget.h"
#include "UI/GOLobbyTeamMembersWidget.h"
#include "Interface/GOLobbyHUDInterface.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"

UGOLobbyHUDWidget::UGOLobbyHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGOLobbyHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HeroSelectionWidget = Cast<UGOHeroSelectionWidget>(GetWidgetFromName(TEXT("WidgetHeroSelection")));
	ensure(HeroSelectionWidget);
	HeroSelectionWidget->SetVisibility(ESlateVisibility::Hidden);

	WidgetTeamMembers = Cast<UGOLobbyTeamMembersWidget>(GetWidgetFromName(TEXT("TeamMembersWidget")));
	ensure(WidgetTeamMembers);
	WidgetTeamMembers->SetVisibility(ESlateVisibility::Hidden);

	CountdownText = Cast<UCommonTextBlock>(GetWidgetFromName(TEXT("CountdownText")));
	ensure(CountdownText);
	CountdownText->SetVisibility(ESlateVisibility::Hidden);

	IGOLobbyHUDInterface* HUDController = Cast<IGOLobbyHUDInterface>(GetOwningPlayer());
	if (HUDController)
	{
		HUDController->SetupLobbyHUDWidget(this);
	}

	BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
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
	BackgroundImage->SetVisibility(ESlateVisibility::Visible);
	WidgetTeamMembers->SetVisibility(ESlateVisibility::Visible);
}

void UGOLobbyHUDWidget::HideHeroSelectionWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("UGOLobbyHUDWidget::HideHeroSelectionWidget() "));

	if (HeroSelectionWidget)
	{
		HeroSelectionWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
	WidgetTeamMembers->SetVisibility(ESlateVisibility::Hidden);
	UE_LOG(LogTemp, Warning, TEXT("UGOLobbyHUDWidget::HideHeroSelectionWidget() 2 "));
}

void UGOLobbyHUDWidget::SetTeamText(ETeamType TeamType)
{
	WidgetTeamMembers->SetTeamText(TeamType);
}
