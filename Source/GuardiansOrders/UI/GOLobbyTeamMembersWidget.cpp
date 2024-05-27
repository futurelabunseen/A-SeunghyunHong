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
        UE_LOG(LogTemp, Warning, TEXT("UGOLobbyTeamMembersWidget SetTeamText %d"), TeamType); // O

        if (TeamType == ETeamType::ET_RedTeam)
        {
            FText NewTeamText = FText::FromString(TEXT("Red"));
            TeamText->SetText(NewTeamText);
        }
        else if (TeamType == ETeamType::ET_BlueTeam)
        {
            FText NewTeamText = FText::FromString(TEXT("Blue"));
            TeamText->SetText(NewTeamText);
        }
    }
}
