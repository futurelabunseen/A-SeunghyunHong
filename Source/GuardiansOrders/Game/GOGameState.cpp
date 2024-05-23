// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOGameState.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h" 
#include "UObject/ConstructorHelpers.h"
#include "CommonUserWidget.h"
#include "Game/GOPlayerState.h"
#include <Player/GOLobbyPlayerController.h>
#include "UI/GOLobbyHUDWidget.h"
#include "CommonTextBlock.h"
#include "Player/GOPlayerController.h"

AGOGameState::AGOGameState()
{
	RemainingTime = MatchPlayTime;

	RemainingReadyTravelTime = 0;
	bShowHeroSelectionWidget = false;
}

void AGOGameState::HandleBeginPlay()
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOGameState::OnRep_ReplicatedHasBegunPlay()
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AGOGameState, RemainingTime);
	DOREPLIFETIME(AGOGameState, bShowHeroSelectionWidget);
	DOREPLIFETIME(AGOGameState, RedTeamScore);
	DOREPLIFETIME(AGOGameState, BlueTeamScore);
	DOREPLIFETIME(AGOGameState, RedTeamHeroes);
	DOREPLIFETIME(AGOGameState, BlueTeamHeroes);
	DOREPLIFETIME(AGOGameState, RemainingReadyTravelTime);
	DOREPLIFETIME(AGOGameState, TopScoringPlayers);
}

void AGOGameState::OnRep_CharacterSelected()
{
	// Update UI 

	if (GEngine)
	{
		// Red Team
		for (const FHeroSelectionInfo& HeroInfo : RedTeamHeroes)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan,
				FString::Printf(TEXT("[AGOGameState OnRep_CharacterSelected] Red Team - PlayerId: %d, HeroType: %d"),
					HeroInfo.PlayerId, static_cast<int32>(HeroInfo.SelectedHero)));
		}

		// Blue Team
		for (const FHeroSelectionInfo& HeroInfo : BlueTeamHeroes)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan,
				FString::Printf(TEXT("[AGOGameState OnRep_CharacterSelected] Blue Team - PlayerId: %d, HeroType: %d"),
					HeroInfo.PlayerId, static_cast<int32>(HeroInfo.SelectedHero)));
		}
	}



}

void AGOGameState::OnRep_CountDownForTravelReadyTime()
{

}

void AGOGameState::RedTeamScores()
{
	++RedTeamScore;
	AGOPlayerController* GOPlayer = Cast<AGOPlayerController>(GetWorld()->GetFirstPlayerController());
	if (GOPlayer)
	{
		GOPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void AGOGameState::BlueTeamScores()
{
	++BlueTeamScore;
	AGOPlayerController* GOPlayer = Cast<AGOPlayerController>(GetWorld()->GetFirstPlayerController());
	if (GOPlayer)
	{
		GOPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}

void AGOGameState::OnRep_RedTeamScore()
{
	AGOPlayerController* GOPlayer = Cast<AGOPlayerController>(GetWorld()->GetFirstPlayerController());
	if (GOPlayer)
	{
		GOPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void AGOGameState::OnRep_BlueTeamScore()
{
	AGOPlayerController* GOPlayer = Cast<AGOPlayerController>(GetWorld()->GetFirstPlayerController());
	if (GOPlayer)
	{
		GOPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}

void AGOGameState::UpdateTopscore(AGOPlayerState* ScoringPlayer)
{
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if (ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}

void AGOGameState::OnRep_HeroSelectionWidget()
{
	ShowHeroSelectionWidget();
}

void AGOGameState::OnGamePlayerReadyNotified()
{
	if (HasAuthority())
	{
		bShowHeroSelectionWidget = true;
		ShowHeroSelectionWidget();
	}
}

void AGOGameState::ShowHeroSelectionWidget()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController && PlayerController->IsLocalController())
	{
		DisplayHeroSelectionWidget(PlayerController);
		UE_LOG(LogTemp, Warning, TEXT("[GAME STATE] ShowHeroSelectionWidget player %s"),*PlayerController->GetName());
	}
}

bool AGOGameState::AreAllPlayersReady()
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		AGOPlayerState* GOPlayerState = Cast<AGOPlayerState>(PlayerState);
		if (GOPlayerState && !GOPlayerState->bIsReady)
		{
			return false;
		}
	}
	return true;
}

void AGOGameState::DisplayHeroSelectionWidget(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		AGOLobbyPlayerController* LobbyController = Cast<AGOLobbyPlayerController>(PlayerController);
		if (LobbyController && LobbyController->GOLobbyHUDWidget)
		{
			LobbyController->GOLobbyHUDWidget->ShowHeroSelectionWidget();
			UE_LOG(LogTemp, Warning, TEXT("[GAME STATE] HeroSelectionWidget set to visible for player %s"), *PlayerController->GetName());

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
					FString::Printf(TEXT("[GAME STATE] HeroSelectionWidget set to visible for player %s"), *PlayerController->GetName()));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[GAME STATE] Failed to find GOLobbyHUDWidget for player %s"), *PlayerController->GetName());
		}
	}
}