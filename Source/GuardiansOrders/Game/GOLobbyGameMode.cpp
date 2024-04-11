// Fill out your copyright notice in the Description page of Project Settings.


#include "GOLobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GOGameState.h"
#include "GOPlayerState.h"

AGOLobbyGameMode::AGOLobbyGameMode()
{
	GameStateClass = AGOGameState::StaticClass();
	PlayerStateClass = AGOPlayerState::StaticClass();
}

void AGOLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	int32 NumOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Map/Battle?listen"));
		}
	}
}
