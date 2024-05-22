// Fill out your copyright notice in the Description page of Project Settings.


#include "GOLobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GOGameState.h"
#include "GOPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Character/GOPlayerCharacter.h"
#include "Player/GOLobbyPlayerController.h"
#include "Player/GOPlayerController.h"
#include "Cheats/GOCheatManager.h"

AGOLobbyGameMode::AGOLobbyGameMode()
{
	GameStateClass = AGOGameState::StaticClass();
	PlayerStateClass = AGOPlayerState::StaticClass();
	//CheatClass = UGOCheatManager::StaticClass();
}

void AGOLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

// 폰이 잇으면 제거하고 월드에 스폰해서 한다
// 들어오는 애마다
// 한명씩 되자나요 
// 선택한 정보로 그거를 빙의하도록 해요 
void AGOLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	SetupInputMode(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOLobbyGameMode PostLogin : %s"), *NewPlayer->GetName());

	if (NewPlayer && NewPlayer->PlayerState)
	{
		FString PlayerName = NewPlayer->PlayerState->GetPlayerName();
		FString PlayerControllerName = NewPlayer->GetName();

		// Display the player's name on the screen
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green,
				FString::Printf(TEXT("Player %s (Controller: %s) has logged in"),
					*PlayerName, *PlayerControllerName));
		}
	}

	if (HasAuthority())
	{
		int32 NumOfPlayers = GameState.Get()->PlayerArray.Num();
		if (NumOfPlayers == 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("[LOBBY] Two players logged in, waiting for character selection"));

			OnGamePlayerReady();
		}
	}

	// Set Team
	AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
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
	}
}

void AGOLobbyGameMode::Logout(AController* Exiting)
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

void AGOLobbyGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	if (!C)
	{
		UE_LOG(LogTemp, Error, TEXT("[SEAMLESS] AGOLobbyGameMode::HandleSeamlessTravelPlayer - Controller is null"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOLobbyGameMode::HandleSeamlessTravelPlayer - Controller class: %s"), *C->GetClass()->GetName());

	if (C->IsA(AGOLobbyPlayerController::StaticClass()))
	{
		AGOLobbyPlayerController* OldController = Cast<AGOLobbyPlayerController>(C);
		if (OldController)
		{
			APawn* OldPawn = OldController->GetPawn();
			FVector Location = FVector::ZeroVector;
			FRotator Rotation = FRotator::ZeroRotator;

			if (OldPawn)
			{
				Location = OldPawn->GetActorLocation();
				Rotation = OldPawn->GetActorRotation();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOLobbyGameMode::HandleSeamlessTravelPlayer - OldPawn is null, using default location and rotation"));
				Location = FVector(0.f, 0.f, 100.f);  // 적절한 기본 위치로 설정
				Rotation = FRotator::ZeroRotator;    // 적절한 기본 회전으로 설정
			}

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = OldController;
			SpawnParams.Instigator = OldPawn;

			AGOPlayerController* NewController = GetWorld()->SpawnActor<AGOPlayerController>(AGOPlayerController::StaticClass(), Location, Rotation, SpawnParams);
			if (NewController)
			{
				// Copy properties from the old controller to the new controller
				NewController->PlayerState = OldController->PlayerState;
				OldController->PlayerState->SetOwner(NewController);

				if (OldPawn)
				{
					OldPawn->SetOwner(NewController);
					// Possess the current pawn
					NewController->Possess(OldPawn);
					UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOLobbyGameMode::HandleSeamlessTravelPlayer - NewController->Possess(OldPawn) 1 : %s"), *OldPawn->GetName());

				}
				else
				{
					// 새로운 Pawn 생성
					APawn* NewPawn = GetWorld()->SpawnActor<APawn>(APawn::StaticClass(), Location, Rotation, SpawnParams);
					if (NewPawn)
					{
						NewController->Possess(NewPawn);
						UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOLobbyGameMode::HandleSeamlessTravelPlayer - NewController->Possess(NewPawn) 2 : %s"), *NewPawn->GetName());
					}
				}

				// Replace the old controller with the new controller
				C = NewController;

				// Destroy the old controller
				OldController->Destroy();

				UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOLobbyGameMode::HandleSeamlessTravelPlayer - New PlayerController created and possessed pawn: %s"), *NewController->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOLobbyGameMode::HandleSeamlessTravelPlayer - Failed to spawn NewController"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOLobbyGameMode::HandleSeamlessTravelPlayer - OldController is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] AGOLobbyGameMode::HandleSeamlessTravelPlayer - Controller is not of type AGOLobbyPlayerController"));
	}
}





//void AGOLobbyGameMode::SetSelectedCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass)
//{
//	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
//	if (PlayerController)
//	{
//		AGOPlayerState* PlayerState = PlayerController->GetPlayerState<AGOPlayerState>();
//		if (PlayerState)
//		{
//			PlayerState->SelectedCharacterClass = CharacterClass;
//			PlayerCharacterClasses.Add(PlayerController, CharacterClass);
//			UE_LOG(LogTemp, Warning, TEXT("[SEAMLESS] Lobby SelectedCharacterClass: %s"), *PlayerState->SelectedCharacterClass->GetName());
//
//			CheckAllPlayersSelected();
//		}
//	}
//}

void AGOLobbyGameMode::CheckAllPlayersSelected()
{
	int32 NumOfPlayers = GameState.Get()->PlayerArray.Num();
	//if (PlayerCharacterClasses.Num() == 2) //2
	//{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			//FString URLParams;
			//for (const auto& PlayerClass : PlayerCharacterClasses)
			//{
			//	URLParams += FString::Printf(TEXT("?CharacterClass=%s"), *PlayerClass.Value->GetName());
			//}
			//World->ServerTravel(FString("/Game/Map/MyBattle") + URLParams + TEXT("?listen"));
			World->ServerTravel(FString("/Game/Map/MyBattle") + TEXT("?listen"));
		}
	//}
}

void AGOLobbyGameMode::SetupInputMode(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}

void AGOLobbyGameMode::SpawnPlayerCharacter(APlayerController* NewPlayer, EHeroType HeroType)
{
	UE_LOG(LogTemp, Log, TEXT("[LOBBY] Spawning character for: %s"), *NewPlayer->GetName());

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

		FVector SpawnLocation = FVector(0.0f, 0.0f, 200.0f);  // Set your spawn location
		FRotator SpawnRotation = FRotator::ZeroRotator; // Set your spawn rotation

		AGOPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AGOPlayerCharacter>(CharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewCharacter)
		{
			NewPlayer->Possess(NewCharacter);
			UE_LOG(LogTemp, Log, TEXT("[LOBBY] Player possessed new character: %s"), *NewCharacter->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn player character."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterClass is null! Cannot spawn player character."));
	}
}


void AGOLobbyGameMode::OnGamePlayerReady()
{
	AGOGameState* GS = GetWorld()->GetGameState<AGOGameState>();
	if (GS)
	{
		GS->OnGamePlayerReadyNotified();
	}
}

//void AGOLobbyGameMode::ServerSelectHero_Implementation(APlayerController* PlayerController, EHeroType HeroType)
//{
//	SelectHero(PlayerController, HeroType);
//}
//
//bool AGOLobbyGameMode::ServerSelectHero_Validate(APlayerController* PlayerController, EHeroType HeroType)
//{
//	return true;
//}

void AGOLobbyGameMode::SelectHero(APlayerController* PlayerController, EHeroType HeroType)
{
	UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyGameMode] SelectHero 0 "));

	if (PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyGameMode] SelectHero 1 "));

		AGOPlayerState* PS = PlayerController->GetPlayerState<AGOPlayerState>();
		AGOGameState* GS = GetWorld()->GetGameState<AGOGameState>();

		if (PS && GS)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AGOLobbyGameMode] SelectHero 2 "));

			int32 PlayerId = PS->GetPlayerId();
			PS->SelectedHero = FHeroSelectionInfo(PlayerId, HeroType);

			PS->bIsReady = true;

			GS->RedTeamHeroes.RemoveAll([PlayerId](const FHeroSelectionInfo& Hero) {
				return Hero.PlayerId == PlayerId;
				});
			GS->BlueTeamHeroes.RemoveAll([PlayerId](const FHeroSelectionInfo& Hero) {
				return Hero.PlayerId == PlayerId;
				});

			if (PS->GetTeamType() == ETeamType::ET_RedTeam)
			{
				GS->RedTeamHeroes.Add(PS->SelectedHero);
			}
			else if (PS->GetTeamType() == ETeamType::ET_BlueTeam)
			{
				GS->BlueTeamHeroes.Add(PS->SelectedHero);
			}

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Cyan,
					FString::Printf(TEXT("SelectHero - PlayerId: %d, HeroType: %d"), PlayerId, static_cast<int32>(HeroType)));
			}

			for (const FHeroSelectionInfo& Hero : GS->RedTeamHeroes)
			{
				GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Cyan,
					FString::Printf(TEXT("RedTeam - PlayerId: %d, HeroType: %d"), Hero.PlayerId, static_cast<int32>(Hero.SelectedHero)));
			}

			for (const FHeroSelectionInfo& Hero : GS->BlueTeamHeroes)
			{
				GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Cyan,
					FString::Printf(TEXT("BlueTeam - PlayerId: %d, HeroType: %d"), Hero.PlayerId, static_cast<int32>(Hero.SelectedHero)));
			}

			GS->OnRep_CharacterSelected();

			// 캐릭터 스폰 및 소유
			SpawnPlayerCharacter(PlayerController, HeroType);

			// 모든 플레이어가 Ready 상태인지 확인하고 ServerTravel 실행
			if (GS->AreAllPlayersReady())
			{
				UWorld* World = GetWorld();
				if (World)
				{
					bUseSeamlessTravel = true;
					World->ServerTravel(FString("/Game/Map/MyBattle") + TEXT("?listen"));
				}
			}
		}
	}
}
