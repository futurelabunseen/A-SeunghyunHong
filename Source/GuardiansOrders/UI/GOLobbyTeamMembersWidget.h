// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Share/EGOTeam.h"
#include "Share/ShareEnums.h"
#include "GOLobbyTeamMembersWidget.generated.h"

class UGOLobbyTeamMemberWidget;
class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOLobbyTeamMembersWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UGOLobbyTeamMembersWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetTeamText(ETeamType TeamType);


	UPROPERTY(meta = (BindWidget))
	UGOLobbyTeamMemberWidget* HeroMemeber01;

	UPROPERTY(meta = (BindWidget))
	UGOLobbyTeamMemberWidget* HeroMemeber02;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* TeamText;
};
