// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOLobbyTeamMembersWidget.h"
#include "CommonTextBlock.h"

UGOLobbyTeamMembersWidget::UGOLobbyTeamMembersWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UGOLobbyTeamMembersWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGOLobbyTeamMembersWidget::SetTeamText(ETeamType TeamType)
{
    if (TeamText)
    {
        FText NewTeamText;
        FSlateColor NewColor;

        if (TeamType == ETeamType::ET_RedTeam)
        {
            NewTeamText = FText::FromString(TEXT("Red"));
            NewColor = FSlateColor(FLinearColor::White);
        }
        else if (TeamType == ETeamType::ET_BlueTeam)
        {
            NewTeamText = FText::FromString(TEXT("Blue"));
            NewColor = FSlateColor(FLinearColor::White);
        }

        TeamText->SetText(NewTeamText);
        TeamText->SetColorAndOpacity(NewColor);
    }
}

