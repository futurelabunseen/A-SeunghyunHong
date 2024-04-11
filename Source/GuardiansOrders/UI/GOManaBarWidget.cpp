// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOManaBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/GOCharacterWidgetInterface.h"

UGOManaBarWidget::UGOManaBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxMana = -1.0f;
}

void UGOManaBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ManaProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbManaBar")));
	ensure(ManaProgressBar);

	ManaStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtManaStat")));
	ensure(ManaStat);

	IGOCharacterWidgetInterface* CharacterWidget = Cast<IGOCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UGOManaBarWidget::UpdateStat(const FGOCharacterStat& BaseStat, const FGOCharacterStat& ModifierStat)
{
	MaxMana = (BaseStat + ModifierStat).MaxMana;
	if (ManaProgressBar)
	{
		ManaProgressBar->SetPercent(CurrentMana / MaxMana);
	}
	if (ManaStat)
	{
		ManaStat->SetText(FText::FromString(GetManaStatText()));
	}
}

void UGOManaBarWidget::UpdateManaBar(float NewCurrentMana, float NewMaxMana)
{
	CurrentMana = NewCurrentMana;
	MaxMana = NewMaxMana;

	// MaxHp = NewMaxHp;
	ensure(MaxMana > 0.0f);
	if (ManaProgressBar)
	{
		ManaProgressBar->SetPercent(CurrentMana / MaxMana);
	}

	if (ManaStat)
	{
		ManaStat->SetText(FText::FromString(GetManaStatText()));
	}
}

FString UGOManaBarWidget::GetManaStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentMana, MaxMana);
}
