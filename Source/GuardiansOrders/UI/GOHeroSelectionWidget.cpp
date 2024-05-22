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
    }
}

void UGOHeroSelectionWidget::OnRogersButtonClicked()
{
    // SelectCharacter(AGORogersCharacter::StaticClass());
    SelectCharacter(EHeroType::Rogers);
}

void UGOHeroSelectionWidget::OnKatnissButtonClicked()
{
    // SelectCharacter(AGOKatnissCharacter::StaticClass());
    SelectCharacter(EHeroType::Katniss);
}


void UGOHeroSelectionWidget::OnBeastButtonClicked()
{
    // SelectCharacter(AGOBeastCharacter::StaticClass());
    SelectCharacter(EHeroType::Beast);
}

void UGOHeroSelectionWidget::OnBrideButtonClicked()
{
    // SelectCharacter(AGOBrideCharacter::StaticClass());
    SelectCharacter(EHeroType::Bride);
}


//void UGOHeroSelectionWidget::SelectCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass)
//{
//    //APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
//    //if (PlayerController)
//    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
//    if (PlayerController && PlayerController->IsLocalController())
//    {
//        AGOPlayerState* PlayerState = PlayerController->GetPlayerState<AGOPlayerState>();
//        if (PlayerState)
//        {
//            //PlayerState->SelectCharacter(CharacterClass);
//
//            GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green,
//                FString::Printf(TEXT("[UGOHeroSelectionWidget] Player %s selected ! "),
//                    *PlayerController->GetName()));
//        }
//    }
//}


void UGOHeroSelectionWidget::SelectCharacter(EHeroType HeroType)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        AGOLobbyPlayerController* GOPlayerController = Cast<AGOLobbyPlayerController>(PlayerController);
        if (GOPlayerController)
        {
            GOPlayerController->ServerSelectHero(HeroType);
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
            GOPlayerController->ServerReady(); // Ready 상태를 서버에 전송
        }
    }
}