// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOTeamBattleGameMode.h"
#include "GOGameState.h"
#include "GOPlayerState.h"
#include "Kismet/GameplayStatics.h"

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
}

float AGOTeamBattleGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	AGOPlayerState* AttackerPState = Attacker->GetPlayerState<AGOPlayerState>();
	AGOPlayerState* VictimPState = Victim->GetPlayerState<AGOPlayerState>();
	if (AttackerPState == nullptr || VictimPState == nullptr) return BaseDamage;
	if (VictimPState == AttackerPState)
	{
		return BaseDamage;
	}
	if (AttackerPState->GetTeamType() == VictimPState->GetTeamType())
	{
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
