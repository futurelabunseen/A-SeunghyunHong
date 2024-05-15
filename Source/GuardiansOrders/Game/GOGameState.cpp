// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOGameState.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h" 
#include "GOLobbyGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "CommonUserWidget.h"

AGOGameState::AGOGameState()
{
	RemainingTime = MatchPlayTime;
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

void AGOGameState::OnRep_ShowHeroSelectionWidget()
{
	ShowHeroSelectionWidget();
}

void AGOGameState::DisplayHeroSelectionWidget(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (HeroSelectionWidgetClass)
		{
			UCommonUserWidget* HeroSelectionWidget = CreateWidget<UCommonUserWidget>(PlayerController, HeroSelectionWidgetClass);
			if (HeroSelectionWidget)
			{
				HeroSelectionWidget->AddToViewport();
				UE_LOG(LogTemp, Warning, TEXT("[GAME STATE] %s added to viewport for player %s"), *HeroSelectionWidget->GetName(), *PlayerController->GetName());

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green,
						FString::Printf(TEXT("[GAME STATE] HeroSelectionWidget added to viewport for player %s"), *PlayerController->GetName()));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[GAME STATE] Failed to create HeroSelectionWidget for player %s"), *PlayerController->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[GAME STATE] HeroSelectionWidgetClass is not set."));
		}
	}
}