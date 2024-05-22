// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GOLobbyHUDWidget.generated.h"

class UGOHeroSelectionWidget;
/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOLobbyHUDWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UGOLobbyHUDWidget(const FObjectInitializer& ObjectInitializer);
	void InitializeHeroSelection();
	void ShowHeroSelectionWidget();

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UGOHeroSelectionWidget> HeroSelectionWidget;

};
