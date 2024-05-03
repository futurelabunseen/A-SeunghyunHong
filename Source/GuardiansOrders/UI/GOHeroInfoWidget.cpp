// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOHeroInfoWidget.h"

// HeroIconImage

void UGOHeroInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGOHeroInfoWidget::BindHeroInfo(FGOCharacterData CharacterData)
{
	HeroIconImage->SetBrushFromTexture(CharacterData.HeroIcon);
	// UE_LOG(LogTemp, Warning, TEXT("[HeroInfoUI BindHeroInfo] HeroIconImage is %s"), *CharacterData.HeroIcon.GetName());
}
