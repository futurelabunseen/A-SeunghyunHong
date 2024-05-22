// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GOLobbyPlayerController.h"
#include "Game/GOPlayerState.h"
#include "Game/GOLobbyGameMode.h"

AGOLobbyPlayerController::AGOLobbyPlayerController()
{
    UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyGameMode] AGOLobbyPlayerController() "));
}

void AGOLobbyPlayerController::BeginPlay()
{
    Super::BeginPlay();
    EnableMouseCursor();
}

void AGOLobbyPlayerController::EnableMouseCursor()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
}

void AGOLobbyPlayerController::ServerSelectHero_Implementation(EHeroType HeroType)
{
    UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyGameMode] ServerSelectHero_Implementation() "));

    AGOPlayerState* PS = GetPlayerState<AGOPlayerState>();
    if (PS)
    {
        int32 PlayerId = PS->GetPlayerId();
        PS->SelectedHero = FHeroSelectionInfo(PlayerId, HeroType);

        // È®ÀÎÇÔ
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
                FString::Printf(TEXT("[Controller] ServerSelectHero - PlayerId: %d, HeroType: %d"), PlayerId, static_cast<int32>(PS->SelectedHero.SelectedHero)));
        }
    }
}

bool AGOLobbyPlayerController::ServerSelectHero_Validate(EHeroType HeroType)
{
    return true;
}

void AGOLobbyPlayerController::ServerReady_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyGameMode] AGOLobbyPlayerController ServerReady_Implementation 0 "));

    AGOLobbyGameMode* GOGameMode = Cast<AGOLobbyGameMode>(GetWorld()->GetAuthGameMode());
    if (GOGameMode)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyGameMode] AGOLobbyPlayerController ServerReady_Implementation 1 "));

        GOGameMode->SelectHero(this, GetPlayerState<AGOPlayerState>()->SelectedHero.SelectedHero);
    }
}

bool AGOLobbyPlayerController::ServerReady_Validate()
{
    return true;
}
