// Fill out your copyright notice in the Description page of Project Settings.


#include "GOBattleGameMode.h"
#include "GOGameState.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "GOPlayerState.h"

AGOBattleGameMode::AGOBattleGameMode()
{
	GameStateClass = AGOGameState::StaticClass();
	PlayerStateClass = AGOPlayerState::StaticClass();
}

void AGOBattleGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	// 여기서부터 클라이언트가 새롭게 추가되었다!
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("======================================================="));

	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

APlayerController* AGOBattleGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
	return NewPlayerController;
}

void AGOBattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		if (NetDriver->ClientConnections.Num() == 0)
		{
			GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("No Client Connection"));
		}
		else
		{
			for (const auto& Connection : NetDriver->ClientConnections)
			{
				GO_LOG(LogGONetwork, Log, TEXT("Client Connections: %s"), *Connection->GetName());
			}
		}
	}
	else
	{
		GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOBattleGameMode::StartPlay()
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::StartPlay();
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));

	for (APlayerStart* PlayerStart : TActorRange<APlayerStart>(GetWorld()))
	{
		PlayerStartArray.Add(PlayerStart);
	}
}

void AGOBattleGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &AGOBattleGameMode::DefaultRoundTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void AGOBattleGameMode::DefaultRoundTimer()
{
	AGOGameState* const  GOGameState = Cast<AGOGameState>(GameState);

	if (GOGameState && GOGameState->RemainingTime > 0)
	{
		GOGameState->RemainingTime--;
		GO_LOG(LogGONetwork, Log, TEXT("Remaining  Time: % d"), GOGameState->RemainingTime);

		if (GOGameState->RemainingTime < 0)
		{
			if (GetMatchState() == MatchState::InProgress)
			{
				FinishMatch();
			}
			else  if (GetMatchState() == MatchState::WaitingPostMatch)
			{
				// GetWorld()->ServerTravel("");
			}
		}
	}
}

void AGOBattleGameMode::FinishMatch()
{
	AGOGameState* const GOGameState = Cast<AGOGameState>(GameState);
	if (GOGameState  && IsMatchInProgress())
	{
		EndMatch(); // 현재 매치의 상태가 MatchState::WaitingPostMatch 가 됩니다.
		GOGameState->RemainingTime = GOGameState->ShowResultWaitingTime;
	}
}

FTransform AGOBattleGameMode::GetRandomStartTransform() const
{
	if (PlayerStartArray.Num() == 0)
	{
		return FTransform(FVector(0.0f, 0.0f, 230.0f));
	}
	int32 RandIndex = FMath::RandRange(0, PlayerStartArray.Num() - 1);

	return PlayerStartArray[RandIndex]->GetActorTransform();
}

void AGOBattleGameMode::OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn)
{
}
