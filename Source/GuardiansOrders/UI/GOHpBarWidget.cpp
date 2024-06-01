// Fill out your copyright notice in the Description page of Project Settings.


#include "GOHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/GOCharacterWidgetInterface.h"
#include "Components/Image.h"

UGOHpBarWidget::UGOHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UGOHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);	
	
	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	IGOCharacterWidgetInterface* CharacterWidget = Cast<IGOCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UGOHpBarWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	CurrentHp = NewCurrentHp;
	MaxHp = NewMaxHp;

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UGOHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp);
}

void UGOHpBarWidget::SetHpBarTexture(UTexture2D* NewTexture)
{
	if (HpBarImage && NewTexture)
	{
		HpBarImage->SetBrushFromTexture(NewTexture);
	}
}