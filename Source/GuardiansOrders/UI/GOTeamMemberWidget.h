// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "Share/ShareEnums.h"
#include "GOTeamMemberWidget.generated.h"
class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOTeamMemberWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void SetTeamMemberWidgetVisible(FHeroSelectionInfo HeroInfo);

	UPROPERTY(meta = (BindWidget))
	UImage* HeroIconImage;

	UPROPERTY(meta = (BindWidget))
	UImage* RoundImage;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* NameText;
};
