// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOHUDWidget.h"
#include "Interface/GOCharacterHUDInterface.h"
#include "GOStatsBarWidget.h"
#include "GOHpBarWidget.h"
#include "GOManaBarWidget.h"
#include "GOCharacterStatWidget.h"
#include "SkillWidget/GOSkillSetBarWidget.h"
#include "SpellWidget/GOSpellSetBarWidget.h"
#include "GOHeroInfoWidget.h"
#include <Player/GOPlayerController.h>
#include "UI/GOBattleCharacterOverlayWidget.h"

UGOHUDWidget::UGOHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UGOHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StatsBar = Cast<UGOStatsBarWidget>(GetWidgetFromName(TEXT("WidgetStatsBar")));
	ensure(StatsBar);

	// Access the HpBar and ManaBar through the StatsBar
	HpBar = Cast<UGOHpBarWidget>(StatsBar->GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpBar);

	ManaBar = Cast<UGOManaBarWidget>(StatsBar->GetWidgetFromName(TEXT("PbManaBar")));
	ensure(ManaBar);

	//HpBar = Cast<UGOHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	//ensure(HpBar);

	//ManaBar = Cast<UGOManaBarWidget>(GetWidgetFromName(TEXT("WidgetManaBar")));
	//ensure(ManaBar);

	CharacterStat = Cast<UGOCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	SkillSetBar = Cast<UGOSkillSetBarWidget>(GetWidgetFromName(TEXT("CUI_SkillSetBar")));
	ensure(SkillSetBar);
	UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOHUDWidget] SkillSetBar: %s"), *SkillSetBar.GetName());

	SpellSetBar = Cast<UGOSpellSetBarWidget>(GetWidgetFromName(TEXT("CUI_SpellSetBar")));
	ensure(SpellSetBar);

	HeroInfo = Cast<UGOHeroInfoWidget>(GetWidgetFromName(TEXT("CUI_CharacterInfo")));

	IGOCharacterHUDInterface* HUDPawn = Cast<IGOCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}

void UGOHUDWidget::UpdateStat(const FGOCharacterStat& BaseStat, const FGOCharacterStat& ModifierStat)
{
	FGOCharacterStat TotalStat = BaseStat + ModifierStat;
	// HpBar->UpdateStat(BaseStat, ModifierStat);
	// ManaBar->UpdateStat(BaseStat, ModifierStat);
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UGOHUDWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	HpBar->UpdateHpBar(NewCurrentHp, NewMaxHp);
}

void UGOHUDWidget::UpdateManaBar(float NewCurrentMana, float NewMaxMana)
{
	ManaBar->UpdateManaBar(NewCurrentMana, NewMaxMana);
}

void UGOHUDWidget::AddCharacterOverlay()
{
	UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay 0"));

	APlayerController* BasePlayerController = GetWorld()->GetFirstPlayerController();
	AGOPlayerController* PlayerController = Cast<AGOPlayerController>(BasePlayerController);

	if (PlayerController && CharacterOverlayClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay 1"));

		CharacterOverlay = CreateWidget<UGOBattleCharacterOverlayWidget>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay 2"));

	}
}
