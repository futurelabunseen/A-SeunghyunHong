// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOHeroSelectionWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Character/GORogersCharacter.h"
#include "Character/GOBeastCharacter.h"
#include "Character/GOKatnissCharacter.h"
#include "Character/GOBrideCharacter.h"
#include "Game/GOLobbyGameMode.h"
#include "Game/GOGameState.h"
#include "Game/GOPlayerState.h"
#include "Player/GOLobbyPlayerController.h"
#include "Components/EditableTextBox.h"
#include "Components/HorizontalBox.h"

void UGOHeroSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (RogersButton)
    {
        RogersButton->OnClicked.AddDynamic(this, &UGOHeroSelectionWidget::OnRogersButtonClicked);
    }
    if (BrideButton)
    {
        BrideButton->OnClicked.AddDynamic(this, &UGOHeroSelectionWidget::OnBrideButtonClicked);
    }
    if (BeastButton)
    {
        BeastButton->OnClicked.AddDynamic(this, &UGOHeroSelectionWidget::OnBeastButtonClicked);
    }
    if (KatnissButton)
    {
        KatnissButton->OnClicked.AddDynamic(this, &UGOHeroSelectionWidget::OnKatnissButtonClicked);
    }
    if (ReadyButton)
    {
        ReadyButton->OnClicked.AddDynamic(this, &UGOHeroSelectionWidget::OnReadyButtonClicked);
        ReadyButton->SetIsEnabled(false);
    }
    if (NicknameButton)
    {
        NicknameButton->OnClicked.AddDynamic(this, &UGOHeroSelectionWidget::OnNicknameButtonClicked);
    }
}

void UGOHeroSelectionWidget::OnRogersButtonClicked()
{
    SelectCharacter(EHeroType::Rogers);
}

void UGOHeroSelectionWidget::OnKatnissButtonClicked()
{
    SelectCharacter(EHeroType::Katniss);
}


void UGOHeroSelectionWidget::OnBeastButtonClicked()
{
    SelectCharacter(EHeroType::Beast);
}

void UGOHeroSelectionWidget::OnBrideButtonClicked()
{
    SelectCharacter(EHeroType::Bride);
}

void UGOHeroSelectionWidget::SelectCharacter(EHeroType HeroType)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        AGOLobbyPlayerController* GOPlayerController = Cast<AGOLobbyPlayerController>(PlayerController);
        if (GOPlayerController)
        {
            GOPlayerController->ServerSelectHero(HeroType);
            EnableSetNicknameSection();
        }
    }

}

void UGOHeroSelectionWidget::OnReadyButtonClicked()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        AGOLobbyPlayerController* GOPlayerController = Cast<AGOLobbyPlayerController>(PlayerController);
        if (GOPlayerController)
        {
            //GOPlayerController->DisableAllUI();
            GOPlayerController->ServerReady(); // Ready 상태를 서버에 전송
        }
    }
    this->SetVisibility(ESlateVisibility::Hidden);
}

void UGOHeroSelectionWidget::EnableSetNicknameSection()
{
    if (SetNicknameBox)
    {
        SetNicknameBox->SetVisibility(ESlateVisibility::Visible);
    }
}

void UGOHeroSelectionWidget::EnableReadyButton()
{
    if (ReadyButton)
    {
        ReadyButton->SetVisibility(ESlateVisibility::Visible);
        ReadyButton->SetIsEnabled(true);
    }
}

void UGOHeroSelectionWidget::OnNicknameButtonClicked()
{
    if (EditableNicknameText)
    {
        FString Nickname = EditableNicknameText->GetText().ToString();
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            AGOLobbyPlayerController* GOPlayerController = Cast<AGOLobbyPlayerController>(PlayerController);
            if (GOPlayerController)
            {
                GOPlayerController->ServerRPCConfirmNickname(Nickname);
                EnableReadyButton();

            }
        }
    }
}
