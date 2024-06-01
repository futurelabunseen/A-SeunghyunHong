// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Share/EGOTeam.h"
#include "GOLobbyHUDWidget.generated.h"

class UGOLobbyTeamMembersWidget;
class UGOHeroSelectionWidget;
class UCommonTextBlock;
class UImage;

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
	void HideHeroSelectionWidget();
	void SetTeamText(ETeamType TeamType);

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CountdownText;

	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY()
	TObjectPtr<UGOLobbyTeamMembersWidget> WidgetTeamMembers;

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UGOHeroSelectionWidget> HeroSelectionWidget;


};
