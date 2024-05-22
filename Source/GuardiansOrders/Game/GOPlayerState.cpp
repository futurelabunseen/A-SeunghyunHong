// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Game/GOGameState.h"
#include "Game/GOLobbyGameMode.h"
#include "Character/GOPlayerCharacter.h"
#include "Player/GOLobbyPlayerController.h"

AGOPlayerState::AGOPlayerState()
{
	SelectedCharacterClass = nullptr;
	//bCharacterSelected = false;

	// SetPlayerId()
}

void AGOPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGOPlayerState, SelectedCharacterClass);
	DOREPLIFETIME(AGOPlayerState, SelectedHero);
	DOREPLIFETIME(AGOPlayerState, Team);
	//DOREPLIFETIME(AGOPlayerState, bCharacterSelected);
}

void AGOPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	AGOPlayerState* MyPlayerState = Cast<AGOPlayerState>(PlayerState);
	if (MyPlayerState)
	{
		MyPlayerState->SelectedCharacterClass = SelectedCharacterClass;
		MyPlayerState->SelectedHero = SelectedHero;
		MyPlayerState->Team = Team;
		//MyPlayerState->bCharacterSelected = bCharacterSelected;
	}
}

//void AGOPlayerState::OnRep_CharacterSelected()
//{
//	if (bCharacterSelected)
//	{
//		AGOGameState* GameState = GetWorld()->GetGameState<AGOGameState>();
//		if (GameState)
//		{
//			GameState->CheckAllPlayersSelected();
//		}
//	}
//}

//void AGOPlayerState::SelectCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass)
//{
//	//if (HasAuthority())
//	{
//		SelectedCharacterClass = CharacterClass;
//		bCharacterSelected = true;
//		OnRep_CharacterSelected();
//
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue,
//				FString::Printf(TEXT("[AGOPlayerState] selected %s"),
//					
//					*SelectedCharacterClass->GetName()));
//		}
//	}
//}

void AGOPlayerState::SetTeam(ETeamType TeamToSet)
{
	Team = TeamToSet;

	// 팀에 따른 설정 (예: 매터리얼)
	AGOPlayerCharacter* BCharacter = Cast<AGOPlayerCharacter>(GetPawn());
	if (BCharacter)
	{
		//BCharacter->SetTeamColor(Team);
	}

	if (GEngine)
	{
		FColor TeamColor = (TeamToSet == ETeamType::ET_RedTeam) ? FColor::Red : FColor::Blue;
			GEngine->AddOnScreenDebugMessage(-1, 100.f, TeamColor,
			FString::Printf(TEXT("[AGOPlayerState] Team %d"), TeamToSet));
	}
}

void AGOPlayerState::OnRep_Team()
{
	// 팀에 따른 설정 (예: 매터리얼)
	AGOPlayerCharacter* BCharacter = Cast<AGOPlayerCharacter>(GetPawn());
	if (BCharacter)
	{
		//BCharacter->SetTeamColor(Team);
	}
}