// Fill out your copyright notice in the Description page of Project Settings.


#include "GOBattleGameMode.h"
#include "GOGameState.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "GOPlayerState.h"
#include "Character/GORogersCharacter.h"
#include "Character/GOBeastCharacter.h"
#include "Character/GOKatnissCharacter.h"
#include "Character/GOBrideCharacter.h"
#include "Cheats/GOCheatManager.h"

AGOBattleGameMode::AGOBattleGameMode()
{
	GameStateClass = AGOGameState::StaticClass();
	PlayerStateClass = AGOPlayerState::StaticClass();
	//CheatClass = UGOCheatManager::StaticClass();
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

	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOBattleGameMode::PostSeamlessTravel()
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostSeamlessTravel();

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		APlayerController* PlayerController = PlayerState->GetOwner<APlayerController>();
		if (PlayerController)
		{
			AGOPlayerState* GOPlayerState = Cast<AGOPlayerState>(PlayerState);
			if (GOPlayerState)
			{
				UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] PlayerState found for PlayerController: %s"), *PlayerController->GetName());

				if (GOPlayerState->SelectedCharacterClass)
				{
					SpawnPlayerCharacter(PlayerController, GOPlayerState->SelectedCharacterClass);

					UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] MyBattle SelectedCharacterClass: %s"), *GOPlayerState->SelectedCharacterClass->GetName());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] SelectedCharacterClass is not set. PlayerState: %s, PlayerController: %s"),
						*GOPlayerState->GetName(), *PlayerController->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] PlayerState is invalid. PlayerController: %s"),
					*PlayerController->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] PlayerController is null for PlayerState: %s"),
				*PlayerState->GetName());
		}
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

void AGOBattleGameMode::SpawnPlayerCharacter(APlayerController* NewPlayer, TSubclassOf<AGOPlayerCharacter> CharacterClass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;
	SpawnParams.Instigator = NewPlayer->GetPawn();

	FVector SpawnLocation = GetRandomStartTransform().GetLocation();
	FRotator SpawnRotation = GetRandomStartTransform().GetRotation().Rotator();

	AGOPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AGOPlayerCharacter>(CharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (NewCharacter)
	{
		NewPlayer->Possess(NewCharacter);
	}
}