// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Game/GOGameState.h"
#include "Game/GOLobbyGameMode.h"
#include "Character/GOPlayerCharacter.h"
#include "Player/GOLobbyPlayerController.h"
#include "Player/GOPlayerController.h"
#include "UI/GOLobbyHUDWidget.h"

AGOPlayerState::AGOPlayerState()
{
	SelectedCharacterClass = nullptr;
	//bCharacterSelected = false;

	// SetPlayerId()

	//PlayerName = GetPlayerName();
}

void AGOPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGOPlayerState, SelectedCharacterClass);
	DOREPLIFETIME(AGOPlayerState, SelectedHero);
	DOREPLIFETIME(AGOPlayerState, Team); 
	DOREPLIFETIME(AGOPlayerState, Defeats);
	//DOREPLIFETIME(AGOPlayerState, PlayerName);
	
	//DOREPLIFETIME(AGOPlayerState, bCharacterSelected);
}

void AGOPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	UE_LOG(LogTemp, Warning, TEXT("CopyProperties 000"));

	AGOPlayerState* MyPlayerState = Cast<AGOPlayerState>(PlayerState);
	if (MyPlayerState)
	{
		MyPlayerState->SelectedCharacterClass = SelectedCharacterClass;
		MyPlayerState->SelectedHero = SelectedHero;
		MyPlayerState->Team = Team;
		//MyPlayerState->PlayerName = PlayerName;
		//MyPlayerState->bCharacterSelected = bCharacterSelected;
	}
}

void AGOPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character == nullptr? Cast<AGOPlayerCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AGOPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void AGOPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);

	Character = Character == nullptr ? Cast<AGOPlayerCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AGOPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void AGOPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<AGOPlayerCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AGOPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void AGOPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	Character = Character == nullptr ? Cast<AGOPlayerCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AGOPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void AGOPlayerState::SetTeam(ETeamType TeamToSet)
{
	Team = TeamToSet;
	UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] SetTeam 00 %d "), Team);
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] SetTeam 00 %d PlayerController not null"), Team);

		AGOLobbyPlayerController* LobbyController = Cast<AGOLobbyPlayerController>(PlayerController);
		if (LobbyController)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] SetTeam 00 %d LobbyController not null"), Team);

			if (LobbyController)
			{
				UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] SetTeam 00 %d GOLobbyHUDWidget not null"), Team);

				LobbyController->SetLobbyTeamInfo(GetTeamType());
				UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] SetTeam 01 %d "), TeamToSet);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] SetTeam 00 %d GOLobbyHUDWidget  null"), Team);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] SetTeam 00 %d LobbyController null"), Team);

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] SetTeam 00 %d PlayerController null"), Team);

	}
	// 팀에 따른 설정 (예: 매터리얼)
	AGOPlayerCharacter* BCharacter = Cast<AGOPlayerCharacter>(GetPawn());
	if (BCharacter)
	{
		//BCharacter->SetTeamColor(Team);
	}

	//if (GEngine)
	//{
	//	FColor TeamColor = (TeamToSet == ETeamType::ET_RedTeam) ? FColor::Red : FColor::Blue;
	//		GEngine->AddOnScreenDebugMessage(-1, 0.f, TeamColor,
	//		FString::Printf(TEXT("[AGOPlayerState] Team %d"), TeamToSet));
	//}

}

void AGOPlayerState::OnRep_Team()
{
	// PlayerController를 통해 GOLobbyHUDWidget의 SetLobbyTeamInfo 호출
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (PlayerController)
	{
		AGOLobbyPlayerController* LobbyController = Cast<AGOLobbyPlayerController>(PlayerController);
		if (LobbyController && LobbyController->GOLobbyHUDWidget)
		{
			LobbyController->SetLobbyTeamInfo(GetTeamType());
			UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] SetTeam 11 %d "), GetTeamType());
		}
	}
	//// 팀에 따른 설정 (예: 매터리얼)
	//AGOPlayerCharacter* BCharacter = Cast<AGOPlayerCharacter>(GetPawn());
	//if (BCharacter)
	//{
	//	//BCharacter->SetTeamColor(Team);

	//	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	//	if (PlayerController)
	//	{
	//		AGOLobbyPlayerController* LobbyController = Cast<AGOLobbyPlayerController>(PlayerController);
	//		if (LobbyController && LobbyController->GOLobbyHUDWidget)
	//		{
	//			LobbyController->SetLobbyTeamInfo(GetTeamType());
	//			UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] GetTeamType() : %d"), GetTeamType());
	//		}
	//	}
	//}
}