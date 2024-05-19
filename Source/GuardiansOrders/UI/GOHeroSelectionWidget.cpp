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
}

void UGOHeroSelectionWidget::OnRogersButtonClicked()
{
    //AGOLobbyGameMode* LobbyGameMode = Cast<AGOLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    //if (LobbyGameMode)
    //{
    //    LobbyGameMode->SetSelectedCharacter(AGORogersCharacter::StaticClass());
    //}
    SelectCharacter(AGORogersCharacter::StaticClass());
}

void UGOHeroSelectionWidget::OnKatnissButtonClicked()
{
    //AGOLobbyGameMode* LobbyGameMode = Cast<AGOLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    //if (LobbyGameMode)
    //{
    //    LobbyGameMode->SetSelectedCharacter(AGOKatnissCharacter::StaticClass());
    //}
    SelectCharacter(AGOKatnissCharacter::StaticClass());
}

void UGOHeroSelectionWidget::SelectCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass)
{
    //APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    //if (PlayerController)
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController && PlayerController->IsLocalController())
    {
        AGOPlayerState* PlayerState = PlayerController->GetPlayerState<AGOPlayerState>();
        if (PlayerState)
        {
            //PlayerState->SelectCharacter(CharacterClass);
        
            GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green,
                FString::Printf(TEXT("[UGOHeroSelectionWidget] Player %s selected ! "),
                    *PlayerController->GetName()));
        }
    }
}

void UGOHeroSelectionWidget::OnBeastButtonClicked()
{
    //AGOLobbyGameMode* LobbyGameMode = Cast<AGOLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    //if (LobbyGameMode)
    //{
    //    LobbyGameMode->SetSelectedCharacter(AGOBeastCharacter::StaticClass());
    //}
    SelectCharacter(AGOBeastCharacter::StaticClass());
}

void UGOHeroSelectionWidget::OnBrideButtonClicked()
{
    //AGOLobbyGameMode* LobbyGameMode = Cast<AGOLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    //if (LobbyGameMode)
    //{
    //    LobbyGameMode->SetSelectedCharacter(AGOBrideCharacter::StaticClass());
    //}
    SelectCharacter(AGOBrideCharacter::StaticClass());
}