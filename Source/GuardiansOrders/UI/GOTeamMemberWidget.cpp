// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOTeamMemberWidget.h"
#include "CommonTextBlock.h"
#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>

void UGOTeamMemberWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HeroIconImage->SetVisibility(ESlateVisibility::Hidden);
	RoundImage->SetVisibility(ESlateVisibility::Hidden);
	NameText->SetVisibility(ESlateVisibility::Hidden);
}

void UGOTeamMemberWidget::SetTeamMemberWidgetVisible(FHeroSelectionInfo HeroInfo)
{
	HeroIconImage->SetVisibility(ESlateVisibility::Visible);
	RoundImage->SetVisibility(ESlateVisibility::Visible);
	//NameText->SetVisibility(ESlateVisibility::Visible);

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	HeroIconImage->SetBrushFromTexture(GOGameInstance->GetHeroImageByEHeroType(HeroInfo.SelectedHero));
	//NameText->SetText(FText::FromString(PlayerName));
}
