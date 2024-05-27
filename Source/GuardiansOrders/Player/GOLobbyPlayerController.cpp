// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GOLobbyPlayerController.h"
#include "Game/GOPlayerState.h"
#include "Game/GOLobbyGameMode.h"
#include "UI/GOLobbyHUDWidget.h"
#include "UI/GOLobbyTeamMembersWidget.h"

AGOLobbyPlayerController::AGOLobbyPlayerController()
{
	UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyGameMode] AGOLobbyPlayerController() "));
}

void AGOLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	EnableMouseCursor();
	UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyPlayerController] SetupLobbyHUDWidget 000 "));

	if (IsLocalPlayerController() && IsValid(GOLobbyHUDWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyPlayerController] SetupLobbyHUDWidget 111 "));

		GOLobbyHUDWidget = CreateWidget<UGOLobbyHUDWidget>(this, GOLobbyHUDWidgetClass);

		if (GOLobbyHUDWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyPlayerController] SetupLobbyHUDWidget 222 "));

			GOLobbyHUDWidget->AddToViewport();
			SetupLobbyHUDWidget(GOLobbyHUDWidget);
		}
	}
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

void AGOLobbyPlayerController::DisableAllUI()
{
	GOLobbyHUDWidget->HideHeroSelectionWidget();
}

void AGOLobbyPlayerController::SetLobbyTeamInfo(ETeamType TeamType)
{
	UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyPlayerController SetLobbyTeamInfo] SetTeam  TeamType 01 %d "), TeamType);

	if (GOLobbyHUDWidget)
	{
		GOLobbyHUDWidget->SetTeamText(TeamType);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyPlayerController SetLobbyTeamInfo] SetTeam GOLobbyHUDWidget null "));
	}
}

void AGOLobbyPlayerController::SetupLobbyHUDWidget(UGOLobbyHUDWidget* InLobbyHUDWidget)
{
	UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyPlayerController] SetupLobbyHUDWidget 0 "));
	if (InLobbyHUDWidget)
	{
	}
	UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyPlayerController] SetupLobbyHUDWidget 2 "));

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
