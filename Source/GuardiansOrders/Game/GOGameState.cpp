// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOGameState.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "Net/UnrealNetwork.h"

AGOGameState::AGOGameState()
{
	RemainingTime = MatchPlayTime;
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

	DOREPLIFETIME(AGOGameState, RemainingTime);
}

