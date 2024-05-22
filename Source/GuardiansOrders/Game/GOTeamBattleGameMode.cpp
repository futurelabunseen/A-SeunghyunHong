// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOTeamBattleGameMode.h"
#include "GOGameState.h"
#include "GOPlayerState.h"
#include "Kismet/GameplayStatics.h"

void AGOTeamBattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	/*AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		AGOPlayerState* BPState = NewPlayer->GetPlayerState<AGOPlayerState>();
		if (BPState && BPState->GetTeamType() == ETeamType::ET_NoTeam)
		{
			if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
			{
				BGameState->RedTeam.AddUnique(BPState);
				BPState->SetTeam(ETeamType::ET_RedTeam);
			}
			else
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeamType::ET_BlueTeam);
			}
		}
	}*/
}

void AGOTeamBattleGameMode::Logout(AController* Existing)
{
	AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
	AGOPlayerState* BPState = Existing->GetPlayerState<AGOPlayerState>();

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

void AGOTeamBattleGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	/*AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		for (auto PState : BGameState->PlayerArray)
		{
			AGOPlayerState* BPState = Cast<AGOPlayerState>(PState.Get());
			if (BPState && BPState->GetTeamType() == ETeamType::ET_NoTeam)
			{
				if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
				{
					BGameState->RedTeam.AddUnique(BPState);
					BPState->SetTeam(ETeamType::ET_RedTeam);
				}
				else
				{
					BGameState->BlueTeam.AddUnique(BPState);
					BPState->SetTeam(ETeamType::ET_BlueTeam);
				}
			}
		}
	}*/
}


/*

	AGOPlayerState* PS = NewPlayer->GetPlayerState<AGOPlayerState>();
	int32 PlayerId = PS->GetPlayerId();
	PS->SelectedHero = FHeroSelectionInfo(PlayerId);

	if (PS->GetTeamType() == ETeamType::ET_RedTeam)
	{
		GEngine->AddOnScreenDebugMessage(-1, 40.f, FColor::Cyan,
				FString::Printf(TEXT("RedTeam - PlayerId: %d"), PlayerId));
	}
	else if (PS->GetTeamType() == ETeamType::ET_BlueTeam)
	{
		GEngine->AddOnScreenDebugMessage(-1, 40.f, FColor::Cyan,
			FString::Printf(TEXT("BlueTeam - PlayerId: %d"), PlayerId));
	}

*/