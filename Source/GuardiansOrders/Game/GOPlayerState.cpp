// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Game/GOGameState.h"
#include "Game/GOLobbyGameMode.h"
#include "Character/GOPlayerCharacter.h"
#include "Player/GOLobbyPlayerController.h"
#include "Player/GOPlayerController.h"

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
	DOREPLIFETIME(AGOPlayerState, Defeats);
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

	// ���� ���� ���� (��: ���͸���)
	AGOPlayerCharacter* BCharacter = Cast<AGOPlayerCharacter>(GetPawn());
	if (BCharacter)
	{
		//BCharacter->SetTeamColor(Team);
	}

	if (GEngine)
	{
		FColor TeamColor = (TeamToSet == ETeamType::ET_RedTeam) ? FColor::Red : FColor::Blue;
			GEngine->AddOnScreenDebugMessage(-1, 10.f, TeamColor,
			FString::Printf(TEXT("[AGOPlayerState] Team %d"), TeamToSet));
	}
}

void AGOPlayerState::OnRep_Team()
{
	// ���� ���� ���� (��: ���͸���)
	AGOPlayerCharacter* BCharacter = Cast<AGOPlayerCharacter>(GetPawn());
	if (BCharacter)
	{
		//BCharacter->SetTeamColor(Team);
	}
}