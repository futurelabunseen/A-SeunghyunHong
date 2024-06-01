// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOLobbyTeamMemberWidget.h"
#include "Components/Overlay.h"
#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include "CommonTextBlock.h"

UGOLobbyTeamMemberWidget::UGOLobbyTeamMemberWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UGOLobbyTeamMemberWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RingOverlay->SetVisibility(ESlateVisibility::Visible);

	RingOverlay = Cast<UOverlay>(GetWidgetFromName(TEXT("RingOverlay")));
	ensure(RingOverlay);
}
void UGOLobbyTeamMemberWidget::SetTeamMemberUI(FHeroSelectionInfo HeroInfo)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	HeroIconImage->SetBrushFromTexture(GOGameInstance->GetHeroImageByEHeroType(HeroInfo.SelectedHero));
	HeroNameText->SetText(GetEHeroTypeAsText(HeroInfo.SelectedHero));
}
