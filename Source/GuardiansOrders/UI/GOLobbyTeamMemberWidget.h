// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GOLobbyTeamMemberWidget.generated.h"

class UImage;
class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOLobbyTeamMemberWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage* HeroIconImage;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* HeroNameText;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* SteamNameText;
};
