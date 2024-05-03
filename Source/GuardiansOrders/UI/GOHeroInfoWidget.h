// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "GameData/GOCharacterData.h"
#include "GOHeroInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOHeroInfoWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UImage* HeroIconImage;

	void BindHeroInfo(FGOCharacterData CharacterData);
};
