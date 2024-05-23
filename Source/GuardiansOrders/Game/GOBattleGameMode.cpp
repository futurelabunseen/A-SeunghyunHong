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
#include "Player/GOLobbyPlayerController.h"
#include "Player/GOPlayerController.h"

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
	UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode PostLogin PlayerController: %s"), *NewPlayer->GetName());

	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOBattleGameMode::PostSeamlessTravel()
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostSeamlessTravel();
	UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode PostSeamlessTravel"));

	int32 PlayerCount = GameState->PlayerArray.Num();
	UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] Number of players in GameState->PlayerArray: %d"), PlayerCount);

	// 일정 시간 후에 PlayerController를 재검사하기 위해 타이머 설정
	//GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AGOBattleGameMode::CheckPlayerControllers);
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOBattleGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	if (!C)
	{
		UE_LOG(LogTemp, Error, TEXT("[SEAMLESS] AGOBattleGameMode::HandleSeamlessTravelPlayer - Controller is null"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode::HandleSeamlessTravelPlayer - Controller class: %s"), *C->GetClass()->GetName());

	if (AGOPlayerController* PlayerController = Cast<AGOPlayerController>(C))
	{
		if (AGOPlayerState* PlayerState = PlayerController->GetPlayerState<AGOPlayerState>())
		{
			if (!PlayerController->GetPawn())
			{
				// 여기서 스폰빙의
				//UE_LOG(LogTemp, Error, TEXT("[SEAMLESS] AGOBattleGameMode::HandleSeamlessTravelPlayer -SpawnPlayerCharacter"));
				// 선택된 히어로 타입으로 새로운 캐릭터 스폰 및 빙의
				SpawnPlayerCharacter(PlayerController, PlayerState->SelectedHero.SelectedHero);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode::HandleSeamlessTravelPlayer - Player already has a pawn, skipping spawn"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[SEAMLESS] AGOBattleGameMode::HandleSeamlessTravelPlayer - PlayerState is null for Controller: %s"), *C->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode::HandleSeamlessTravelPlayer - Controller is not of type AGOPlayerController"));
	}
}

void AGOBattleGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	//AGOGameState* GameState = GetWorld()->GetGameState<AGOGameState>();
	//if (GameState)
	//{
	//	// 플레이어 상태 정리
	//}

	UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode Logout PlayerController: %s"), *Exiting->GetName());
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
		return FTransform(FVector(600.f, 3600.f, -395.f));
	}
	int32 RandIndex = FMath::RandRange(0, PlayerStartArray.Num() - 1);

	return PlayerStartArray[RandIndex]->GetActorTransform();
}

void AGOBattleGameMode::OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn)
{
	// TODO
	AGOPlayerState* AttackPlayerState = Killer ? Cast<AGOPlayerState>(Killer->PlayerState) : nullptr;
	AGOPlayerState* VictimPlayerState = KilledPlayer ? Cast<AGOPlayerState>(KilledPlayer->PlayerState) : nullptr;

	if (AttackPlayerState && AttackPlayerState != VictimPlayerState)
	{
		AttackPlayerState->AddToScore(1.0f);
	}
}

void AGOBattleGameMode::CheckPlayerControllers()
{
	TArray<APlayerState*> PlayerStates = GameState->PlayerArray;

	for (APlayerState* PlayerState : PlayerStates)
	{
		if (PlayerState)
		{
			if (AGOPlayerController* PlayerController = Cast<AGOPlayerController>(PlayerState->GetOwner()))
			{
				if (!PlayerController->GetPawn())
				{
					AGOPlayerState* GOPlayerState = Cast<AGOPlayerState>(PlayerState);
					if (GOPlayerState)
					{
						UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode CheckPlayerControllers - PlayerState %s found for PlayerController: %s"), *PlayerState->GetName(), *PlayerController->GetName());

						UE_LOG(LogTemp, Error, TEXT("[SEAMLESS] AGOBattleGameMode::CheckPlayerControllers -SpawnPlayerCharacter"));

						// 선택된 히어로 타입으로 새로운 캐릭터 스폰 및 빙의
						SpawnPlayerCharacter(PlayerController, GOPlayerState->SelectedHero.SelectedHero);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode CheckPlayerControllers - PlayerState is invalid. PlayerController: %s"), *PlayerController->GetName());
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode CheckPlayerControllers - Player already has a pawn, skipping spawn"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[SEAMLESS] AGOBattleGameMode CheckPlayerControllers - PlayerController is null for PlayerState: %s"), *PlayerState->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[SEAMLESS] AGOBattleGameMode CheckPlayerControllers - PlayerState is null"));
		}
	}
}

void AGOBattleGameMode::SpawnPlayerCharacter(APlayerController* NewPlayer, EHeroType HeroType)
{
	UE_LOG(LogTemp, Log, TEXT("[SEAMLESS] NewPlayer: %s"), *NewPlayer->GetName());

	TSubclassOf<AGOPlayerCharacter> CharacterClass;
	switch (HeroType)
	{
	case EHeroType::Rogers:
		CharacterClass = RogersCharacterClass;
		break;
	case EHeroType::Katniss:
		CharacterClass = KatnissCharacterClass;
		break;
	case EHeroType::Beast:
		CharacterClass = BeastCharacterClass;
		break;
	case EHeroType::Bride:
		CharacterClass = BrideCharacterClass;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid hero type!"));
		return;
	}

	if (CharacterClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = NewPlayer;
		SpawnParams.Instigator = NewPlayer->GetPawn();

		FVector SpawnLocation = GetRandomStartTransform().GetLocation();
		FRotator SpawnRotation = GetRandomStartTransform().GetRotation().Rotator();

		APawn* OldPawn = NewPlayer->GetPawn();
		if (OldPawn)
		{
			OldPawn->Destroy();
		}

		AGOPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AGOPlayerCharacter>(CharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
		//UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode::SpawnPlayerCharacter - NewCharacter %s"), *NewCharacter->GetName());

		if (NewCharacter)
		{
			NewPlayer->Possess(NewCharacter);
			UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode::SpawnPlayerCharacter - %s->Possess(%s) "), *NewPlayer->GetName(), *NewCharacter->GetName());

			FTransform StartTransform = GetRandomStartTransform();
			NewCharacter->SetActorLocation(StartTransform.GetLocation());
			NewCharacter->SetActorRotation(StartTransform.GetRotation().Rotator());

			FVector PlayerStartLocation = StartTransform.GetLocation();
			UE_LOG(LogTemp, Log, TEXT("[SEAMLESS]  check!! AGOBattleGameMode::SpawnPlayerCharacter - Player spawned and moved to PlayerStart Location: X=%f, Y=%f, Z=%f"),
				PlayerStartLocation.X, PlayerStartLocation.Y, PlayerStartLocation.Z);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS]  check!!  AGOBattleGameMode::SpawnPlayerCharacter - Failed to spawn player character."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOBattleGameMode::SpawnPlayerCharacter - CharacterClass is null! Cannot spawn player character."));
	}
}