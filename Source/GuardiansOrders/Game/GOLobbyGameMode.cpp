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

AGOLobbyGameMode::AGOLobbyGameMode()
{
	GameStateClass = AGOGameState::StaticClass();
	PlayerStateClass = AGOPlayerState::StaticClass();
}

void AGOLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HeroSelectionWidgetClass)
	{
		UCommonUserWidget* HeroSelectionWidget = CreateWidget<UCommonUserWidget>(GetWorld(), HeroSelectionWidgetClass);
		if (HeroSelectionWidget)
		{
			HeroSelectionWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("[LOBBY] HeroSelectionWidget added to viewport"));

		}
	}
}

void AGOLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	SetupInputMode(NewPlayer);

	int32 NumOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumOfPlayers == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LOBBY] Two players logged in, waiting for character selection"));

		//UWorld* World = GetWorld();
		//if (World)
		//{
		//	bUseSeamlessTravel = true;
		//	//World->ServerTravel(FString("/Game/Map/Battle?listen"));

		//	for (const auto& PlayerSelection : PlayerCharacterSelection)
		//	{
		//		FString URLParams = FString::Printf(TEXT("?CharacterType=%d"), static_cast<int32>(PlayerSelection.Value));
		//		World->ServerTravel(FString("/Game/Map/Battle") + URLParams + TEXT("?listen"));
		//	}
		//}
	}
}

void AGOLobbyGameMode::SetSelectedCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FVector SpawnLocation = FVector::ZeroVector; // 원하는 스폰 위치 설정
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PlayerController;

		AGOPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AGOPlayerCharacter>(CharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewCharacter)
		{
			PlayerController->Possess((APawn*)NewCharacter);

			// Add
			//AGOPlayerState* PlayerState = PlayerController->GetPlayerState<AGOPlayerState>();
			//if (PlayerState)
			//{
			//	PlayerState->SelectedCharacterClass = CharacterClass;
			//	PlayerCharacterSelection.Add(PlayerController, NewCharacter->MyHeroType);
			//	CheckAllPlayersSelected();
			//}
		}
	}
}

void AGOLobbyGameMode::CheckAllPlayersSelected()
{
	// Add
	int32 NumOfPlayers = GameState.Get()->PlayerArray.Num();
	if (PlayerCharacterSelection.Num() == NumOfPlayers)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			FString URLParams;
			for (const auto& PlayerSelection : PlayerCharacterSelection)
			{
				URLParams += FString::Printf(TEXT("?CharacterType=%d"), static_cast<int32>(PlayerSelection.Value));
			}
			World->ServerTravel(FString("/Game/Map/Battle") + URLParams + TEXT("?listen"));
		}
	}
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
