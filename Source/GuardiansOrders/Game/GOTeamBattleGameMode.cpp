// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOTeamBattleGameMode.h"
#include "GOGameState.h"
#include "GOPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include <EngineUtils.h>
#include "GameFramework/PlayerStart.h"
#include <Character/GOCharacterBase.h>
#include "Player/GOPlayerController.h"

namespace  MatchState
{
	const FName Cooldown = FName("Cooldown");
	const FName RoundEnd = FName("RoundEnd");
}

AGOTeamBattleGameMode::AGOTeamBattleGameMode()
{
	bTeamsMatch = true;
	GameStateClass = AGOGameState::StaticClass();
	PlayerStateClass = AGOPlayerState::StaticClass();
}

void AGOTeamBattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AGOTeamBattleGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
	AGOPlayerState* BPState = Exiting->GetPlayerState<AGOPlayerState>();

	if (BGameState && BPState)
	{
		if (BGameState->RedTeam.Contains(BPState))
		{
			BGameState->RedTeam.Remove(BPState);
		}
		if (BGameState->BlueTeam.Contains(BPState))
		{
			BGameState->BlueTeam.Remove(BPState);
		}
	}
}

void AGOTeamBattleGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
}

void AGOTeamBattleGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);
}

void AGOTeamBattleGameMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(LogTemp, Warning, TEXT("[Sequence] AGOTeamBattleGameMode StartPlay"));



	// 모든 플레이어 상태를 가져옴
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AGOPlayerState* GOPlayerState = Cast<AGOPlayerState>(PlayerState);
		if (GOPlayerState)
		{
			ETeamType Team = GOPlayerState->GetTeamType();
			APlayerController* PlayerController = Cast<APlayerController>(GOPlayerState->GetOwner());

			if (PlayerController && PlayerController->GetPawn())
			{
				FVector SpawnLocation;
				FRotator SpawnRotation;

				if (Team == ETeamType::ET_BlueTeam && BlueTeamPlayerStarts.Num() > 0)
				{
					int32 RandIndex = FMath::RandRange(0, BlueTeamPlayerStarts.Num() - 1);
					SpawnLocation = BlueTeamPlayerStarts[RandIndex]->GetActorLocation();
					SpawnRotation = BlueTeamPlayerStarts[RandIndex]->GetActorRotation();
				}
				else if (Team == ETeamType::ET_RedTeam && RedTeamPlayerStarts.Num() > 0)
				{
					int32 RandIndex = FMath::RandRange(0, RedTeamPlayerStarts.Num() - 1);
					SpawnLocation = RedTeamPlayerStarts[RandIndex]->GetActorLocation();
					SpawnRotation = RedTeamPlayerStarts[RandIndex]->GetActorRotation();
				}
				else
				{
					continue; // 팀에 맞는 시작 위치가 없으면 무시
				}

				// 플레이어의 위치를 설정
				APawn* Pawn = PlayerController->GetPawn();
				Pawn->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
				UE_LOG(LogTemp, Warning, TEXT("[TeamBattle] %s moved to Team %d Location: %s"),
					*GOPlayerState->GetPlayerName(), Team, *SpawnLocation.ToString());

				//// Update player's widget nickname
				//AGOCharacterBase* Character = Cast<AGOCharacterBase>(Pawn);
				//if (Character)
				//{
				//	FTimerHandle TimerHandle;
				//	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Character, GOPlayerState]() {
				//		if (Character)
				//		{
				//			Character->UpdateNicknameWidget(GOPlayerState->SelectedHero.PlayerName);
				//		}
				//		}, 3.0f, false);
				//}

				//UE_LOG(LogTemp, Warning, TEXT("[TeamBattle] %s moved to Team %d Location: %s"),
				//	*GOPlayerState->GetPlayerName(), Team, *SpawnLocation.ToString());
			}
		}
	}
}

void AGOTeamBattleGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("[Sequence] AGOTeamBattleGameMode BeginPlay")); // O

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

//void AGOTeamBattleGameMode::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	if (MatchState == MatchState::InProgress)
//	{
//		//UE_LOG(LogTemp, Warning, TEXT("[MatchState] AGOTeamBattleGameMode InProgress")); // O
//		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
//	}
//}

void AGOTeamBattleGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::InProgress)
	{
		UE_LOG(LogTemp, Warning, TEXT("[MatchState] AGOTeamBattleGameMode InProgress")); // O

		//CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;

		// Check if 5 seconds have passed since the match started
		float ElapsedTime = GetWorld()->GetTimeSeconds() - LevelStartingTime;
		if (ElapsedTime >= CooldownTime)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}

	else if (MatchState == MatchState::Cooldown)
	{
		TArray<APlayerState*> PlayerStates = GameState->PlayerArray;

		//for (APlayerState* PlayerState : PlayerStates)
		//{
		//	if (PlayerState)
		//	{
		//		if (AGOPlayerController* PlayerController = Cast<AGOPlayerController>(PlayerState->GetOwner()))
		//		{
		//			AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
		//			if (BGameState)
		//			{
		//				AGOPlayerState* BPState = PlayerController->GetPlayerState<AGOPlayerState>();
		//				if (BPState && BPState->GetTeamType() == ETeamType::ET_BlueTeam)
		//				{
		//					UE_LOG(LogTemp, Warning, TEXT("[SetTeamColor]Cooldown::SetTeamColor"));
		//					BPState->SetTeamColor(ETeamType::ET_BlueTeam);
		//				}
		//				else if (BPState && BPState->GetTeamType() == ETeamType::ET_RedTeam)
		//				{
		//					BPState->SetTeamColor(ETeamType::ET_RedTeam);
		//				}
		//			}
		//		}
		//	}
		//}


		UE_LOG(LogTemp, Warning, TEXT("[MatchState] AGOTeamBattleGameMode Cooldown: %f"), CountdownTime);
		// StartCooldownCountdown();

		if (CountdownTime >= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[MatchState] AGOTeamBattleGameMode Cooldown Countdown 0: %f"), CountdownTime);

			CountdownTime -= DeltaTime;
			UE_LOG(LogTemp, Warning, TEXT("[MatchState] AGOTeamBattleGameMode Cooldown Countdown 1: %f"), CountdownTime);

			// Notify all player controllers to update their HUD
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				AGOPlayerController* PC = Cast<AGOPlayerController>(*It);
				if (PC)
				{
					// UE_LOG(LogTemp, Warning, TEXT("[MatchState] FConstPlayerControllerIterator: %s"), *PC->GetName());

					PC->SetHUDMatchCountdown(CountdownTime);
				}
			}

		}
		else if (CountdownTime < 0)
		{
			
			SetMatchState(MatchState::RoundEnd);
		}
	}

	else if (MatchState == MatchState::RoundEnd)
	{
		UE_LOG(LogTemp, Warning, TEXT("[RoundEnd] AGOTeamBattleGameMode RoundEnd"));

		// 게임 스테이트 가져오기
		AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
		if (BGameState)
		{
			int32 RedTeamScore = BGameState->RedTeamScore;
			int32 BlueTeamScore = BGameState->BlueTeamScore;

			FString WinnerText;

			if (RedTeamScore > BlueTeamScore)
			{
				WinnerText = TEXT("WINNER : RED");
			}
			else if (BlueTeamScore > RedTeamScore)
			{
				WinnerText = TEXT("WINNER : BLUE");
			}
			else
			{
				WinnerText = TEXT("DRAW");
			}

			// 모든 플레이어 컨트롤러에 HUD 업데이트 호출
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				AGOPlayerController* PC = Cast<AGOPlayerController>(*It);
				if (PC)
				{
					PC->SetHUDWinnerText(WinnerText);
				}
			}
		}
	}

}


float AGOTeamBattleGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	AGOPlayerState* AttackerPState = Attacker->GetPlayerState<AGOPlayerState>();
	AGOPlayerState* VictimPState = Victim->GetPlayerState<AGOPlayerState>();
	UE_LOG(LogTemp, Warning, TEXT("[Projectile] CalculateDamage |  AttackerPState : %s , VictimPState : %s "), *AttackerPState->GetName(), *VictimPState->GetName());
	UE_LOG(LogTemp, Warning, TEXT("[Projectile] CalculateDamage Team |  AttackerPState : %d , VictimPState : %d "), AttackerPState->GetTeamType(), VictimPState->GetTeamType());

	if (AttackerPState == nullptr || VictimPState == nullptr) return BaseDamage;
	if (VictimPState == AttackerPState)
	{
		return BaseDamage;
	}
	if (AttackerPState->GetTeamType() == VictimPState->GetTeamType())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Projectile] CalculateDamage Team ==  AttackerPState : %d , VictimPState : %d "), AttackerPState->GetTeamType(), VictimPState->GetTeamType());
		return 0.f;
	}
	return BaseDamage;
}

void AGOTeamBattleGameMode::OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn)
{
	Super::OnPlayerKilled(Killer, KilledPlayer, KilledPawn);

	AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
	AGOPlayerState* AttackerPlayerState = Killer ? Cast<AGOPlayerState>(Killer->PlayerState) : nullptr;
	if (BGameState && AttackerPlayerState)
	{
		if (AttackerPlayerState->GetTeamType() == ETeamType::ET_BlueTeam)
		{
			BGameState->BlueTeamScores();
		}
		if (AttackerPlayerState->GetTeamType() == ETeamType::ET_RedTeam)
		{
			BGameState->RedTeamScores();
		}
		UE_LOG(LogTemp, Warning, TEXT("AttackerPlayerState->GetTeamType() : %d"), AttackerPlayerState->GetTeamType());
	}
}

void AGOTeamBattleGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AGOPlayerController* PC = Cast<AGOPlayerController>(*It);
		if (PC)
		{
			PC->OnMatchStateSet(MatchState);
		}
	}
}

void AGOTeamBattleGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AGOTeamBattleGameMode::DefaultRoundTimer()
{
	Super::DefaultRoundTimer();
}

void AGOTeamBattleGameMode::FinishMatch()
{
}

void AGOTeamBattleGameMode::StartCooldownCountdown()
{
	CountdownTime = 180.f; // 3 minutes countdown
	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&AGOTeamBattleGameMode::HandleCooldownCountdown,
		1.0f,
		true
	);
}

void AGOTeamBattleGameMode::HandleCooldownCountdown()
{

	if (CountdownTime > 0)
	{
		CountdownTime--;
		UE_LOG(LogTemp, Warning, TEXT("Cooldown Countdown: %f"), CountdownTime);

		// Notify all player controllers to update their HUD
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			AGOPlayerController* PC = Cast<AGOPlayerController>(*It);
			if (PC)
			{
				PC->SetHUDMatchCountdown(CountdownTime);
			}
		}
	}
}

