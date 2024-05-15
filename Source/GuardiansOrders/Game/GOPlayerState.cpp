// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GOPlayerState.h"
#include "Net/UnrealNetwork.h"

void AGOPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGOPlayerState, SelectedCharacterClass);
}

void AGOPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	AGOPlayerState* MyPlayerState = Cast<AGOPlayerState>(PlayerState);
	if (MyPlayerState)
	{
		MyPlayerState->SelectedCharacterClass = SelectedCharacterClass;
	}
}
