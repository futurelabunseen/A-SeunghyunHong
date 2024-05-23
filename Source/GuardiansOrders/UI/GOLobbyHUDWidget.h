// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GOLobbyHUDWidget.generated.h"

class UGOHeroSelectionWidget;
class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOLobbyHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGOLobbyHUDWidget(const FObjectInitializer& ObjectInitializer);
	void InitializeHeroSelection();
	void ShowHeroSelectionWidget();

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CountdownText;
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UGOHeroSelectionWidget> HeroSelectionWidget;

};
