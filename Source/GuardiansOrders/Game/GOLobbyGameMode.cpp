// Fill out your copyright notice in the Description page of Project Settings.


#include "GOLobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GOGameState.h"
#include "GOPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Character/GOPlayerCharacter.h"
#include "Cheats/GOCheatManager.h"

AGOLobbyGameMode::AGOLobbyGameMode()
{
	GameStateClass = AGOGameState::StaticClass();
	PlayerStateClass = AGOPlayerState::StaticClass();
	//CheatClass = UGOCheatManager::StaticClass();
}

void AGOLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AGOLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	SetupInputMode(NewPlayer);
	
	if (NewPlayer && NewPlayer->PlayerState)
	{
		FString PlayerName = NewPlayer->PlayerState->GetPlayerName();
		FString PlayerControllerName = NewPlayer->GetName();

		// Display the player's name on the screen
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green, 
				FString::Printf(TEXT("Player %s (Controller: %s) has logged in"), 
					*PlayerName, *PlayerControllerName));
		}
	}

	if (HasAuthority()) // 서버에서만 실행
	{
		//AGOGameState* GS = GetWorld()->GetGameState<AGOGameState>();
		//if (GS)
		//{
		//	GS->ShowHeroSelectionWidget(NewPlayer);
		//}

		int32 NumOfPlayers = GameState.Get()->PlayerArray.Num();
		if (NumOfPlayers == 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("[LOBBY] Two players logged in, waiting for character selection"));
			AGOGameState* GS = GetWorld()->GetGameState<AGOGameState>();
			if (GS)
			{
				GS->ShowHeroSelectionWidget();
			}
		}
	}
}

void AGOLobbyGameMode::SetSelectedCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		AGOPlayerState* PlayerState = PlayerController->GetPlayerState<AGOPlayerState>();
		if (PlayerState)
		{
			PlayerState->SelectedCharacterClass = CharacterClass;
			PlayerCharacterClasses.Add(PlayerController, CharacterClass);
			UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] Lobby SelectedCharacterClass: %s"), *PlayerState->SelectedCharacterClass->GetName());

			CheckAllPlayersSelected();
		}
	}
}

void AGOLobbyGameMode::CheckAllPlayersSelected()
{
	int32 NumOfPlayers = GameState.Get()->PlayerArray.Num();
	if (PlayerCharacterClasses.Num() == 2) //2
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			FString URLParams;
			for (const auto& PlayerClass : PlayerCharacterClasses)
			{
				URLParams += FString::Printf(TEXT("?CharacterClass=%s"), *PlayerClass.Value->GetName());
			}
			World->ServerTravel(FString("/Game/Map/MyBattle") + URLParams + TEXT("?listen"));
		}
	}
}
void AGOLobbyGameMode::SetupInputMode(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}

void AGOLobbyGameMode::ShowHeroSelectionWidget(APlayerController* PlayerController)
{
	// 주석해줌
	//if (HeroSelectionWidgetClass && PlayerController)
	//{
	//	UCommonUserWidget* HeroSelectionWidget = CreateWidget<UCommonUserWidget>(PlayerController, HeroSelectionWidgetClass);
	//	if (HeroSelectionWidget)
	//	{
	//		HeroSelectionWidget->AddToViewport();
	//		UE_LOG(LogTemp, Warning, TEXT("[LOBBY] HeroSelectionWidget added to viewport for player %s"), *PlayerController->GetName());
	//	
	//		if (GEngine)
	//		{
	//			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green, 
	//				FString::Printf(TEXT("[LOBBY] HeroSelectionWidget added to viewport for player %s"), *PlayerController->GetName()));
	//		}
	//	}
	//}
}

void AGOLobbyGameMode::OnGamePlayerReady()
{
}
