// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Game/GOGameState.h"
#include "Game/GOLobbyGameMode.h"
#include "Character/GOPlayerCharacter.h"
#include "Player/GOLobbyPlayerController.h"
#include "Player/GOPlayerController.h"
#include "UI/GOLobbyHUDWidget.h"
#include "UI/GOLobbySelectedHeroInfoWidget.h"
#include "CommonTextBlock.h"
#include <UI/GOBattleCharacterOverlayWidget.h>
#include "UI/GOHUDWidget.h"
#include "Components/Overlay.h"

AGOPlayerState::AGOPlayerState()
{
	SelectedCharacterClass = nullptr;
	//bCharacterSelected = false;
	bReplicates = true;

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
	DOREPLIFETIME(AGOPlayerState, SelectedHeroInfo);
	DOREPLIFETIME(AGOPlayerState, bHasGrindingStone);
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
			CheckForGrindingStone();
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
	//Defeats += DefeatsAmount;
	//Character = Character == nullptr ? Cast<AGOPlayerCharacter>(GetPawn()) : Character;
	//if (Character)
	//{
	//	Controller = Controller == nullptr ? Cast<AGOPlayerController>(Character->Controller) : Controller;
	//	if (Controller)
	//	{
	//		Controller->SetHUDDefeats(Defeats);
	//	}
	//}

	Defeats += DefeatsAmount;
	Character = Character == nullptr ? Cast<AGOPlayerCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AGOPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
			//UGOBattleCharacterOverlayWidget* OverlayWidget = Cast<UGOBattleCharacterOverlayWidget>(Controller->GOHUDWidget->CharacterOverlay);
			//if (OverlayWidget && OverlayWidget->GrindingStoneOverlay)
			//{
			//	OverlayWidget->GrindingStoneOverlay->SetVisibility(ESlateVisibility::Visible);
			//}
			// 킬 추적 로직 추가
			//AGOPlayerState* VictimPlayerState = Cast<AGOPlayerState>(Controller->GetPawn()->GetPlayerState());
			//if (VictimPlayerState)
			//{
			//	AddKilledEnemyPlayer(VictimPlayerState->GetPlayerId());
			//	AGOGameState* GameState = GetWorld()->GetGameState<AGOGameState>();
			//	if (GameState)
			//	{
			//		const TArray<int32>& EnemyPlayerIds = (GetTeamType() == ETeamType::ET_RedTeam) ? GameState->BlueTeamPlayerIds : GameState->RedTeamPlayerIds;
			//		if (HasKilledAllEnemyPlayers(EnemyPlayerIds))
			//		{
			//			// AbrasiveStoneOverlay를 Visible로 설정
			//			UGOBattleCharacterOverlayWidget* OverlayWidget = Cast<UGOBattleCharacterOverlayWidget>(Controller->GOHUDWidget->CharacterOverlay);
			//			if (OverlayWidget && OverlayWidget->GrindingStoneOverlay)
			//			{
			//				OverlayWidget->GrindingStoneOverlay->SetVisibility(ESlateVisibility::Visible);
			//			}
			//		}
			//	}
			//}
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

void AGOPlayerState::SetSelectedHero(EHeroType HeroType)
{
	SelectedHeroInfo = HeroType;
	OnRep_SelectedHeroInfo();
}

void AGOPlayerState::SetNickname(const FString& Nickname)
{
	SelectedHero.PlayerName = Nickname;
	UE_LOG(LogTemp, Warning, TEXT("AGOPlayerState::SetNickname Nickname set to: %s"), *SelectedHero.PlayerName);
	OnRep_SelectedNickname();
}

void AGOPlayerState::OnRep_SelectedHeroInfo()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (PlayerController)
	{
		AGOLobbyPlayerController* LobbyController = Cast<AGOLobbyPlayerController>(PlayerController);
		if (LobbyController && LobbyController->GOLobbyHUDWidget)
		{
			LobbyController->GOLobbyHUDWidget->LobbySelectedHeroInfoWidget->UpdateHeroInfo(SelectedHeroInfo);
			UE_LOG(LogTemp, Warning, TEXT("[AGOPlayerState] SetTeam %d "), GetTeamType());
		}
	}
}

void AGOPlayerState::OnRep_SelectedNickname()
{
	//APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	//if (PlayerController)
	//{
	//	AGOLobbyPlayerController* LobbyController = Cast<AGOLobbyPlayerController>(PlayerController);
	//	if (LobbyController && LobbyController->GOLobbyHUDWidget)
	//	{
	//		FText NicknameText = FText::FromString(SelectedHero.PlayerName);
	//		LobbyController->GOLobbyHUDWidget->LobbySelectedHeroInfoWidget->NameText->SetText(NicknameText);
	//	}
	//}
	if (Character) {
		Character->UpdateNicknameWidget(SelectedHero.PlayerName);
	}
}

void AGOPlayerState::AddKilledEnemyPlayer(int32 pID)
{
	KilledEnemyPlayers.Add(pID);
	UE_LOG(LogTemp, Warning, TEXT("[grinding] AddKilledEnemyPlayer 1 "));
}

bool AGOPlayerState::HasKilledAllEnemyPlayers(const TArray<int32>& EnemyPlayerIds)
{
	UE_LOG(LogTemp, Warning, TEXT("[grinding] AddKilledEnemyPlayer 2 "));

	for (int32 EnemyPlayerId : EnemyPlayerIds)
	{
		if (!KilledEnemyPlayers.Contains(EnemyPlayerId) && KilledEnemyPlayers.Num() != 2)
		{
			return false;
		}
	}
	
	if (KilledEnemyPlayers.Num() != 2)
		return false;

	return true; 
}

void AGOPlayerState::CheckForGrindingStone()
{
	AGOGameState* GameState = GetWorld()->GetGameState<AGOGameState>();
	if (GameState)
	{
		const TArray<int32>& EnemyPlayerIds = (GetTeamType() == ETeamType::ET_RedTeam) ? GameState->BlueTeamPlayerIds : GameState->RedTeamPlayerIds;
		if (HasKilledAllEnemyPlayers(EnemyPlayerIds))
		{
			bHasGrindingStone = true;
			OnRep_HasGrindingStone();
		}
	}
}

void AGOPlayerState::SetGrindingStoneVisible()
{
	if (bHasGrindingStone)
	{
		if (Controller)
		{
			Controller->SetGrindingStoneVisible();
		}
	}
}

void AGOPlayerState::OnRep_HasGrindingStone()
{
	if (bHasGrindingStone)
	{
		if (Controller)
		{
			Controller->SetGrindingStoneVisible();
		}
	}
}