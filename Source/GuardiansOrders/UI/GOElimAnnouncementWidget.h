// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GOElimAnnouncementWidget.generated.h"
class UHorizontalBox;
class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOElimAnnouncementWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	void SetElimAnnouncementText(FString AttackerName, FString VictimName);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> AnnouncementBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> AnnouncementText;
};
