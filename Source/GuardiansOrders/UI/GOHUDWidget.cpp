// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOHUDWidget.h"
#include "Interface/GOCharacterHUDInterface.h"
#include "GOStatsBarWidget.h"
#include "GOHpBarWidget.h"
#include "GOManaBarWidget.h"
#include "GOCharacterStatWidget.h"

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
