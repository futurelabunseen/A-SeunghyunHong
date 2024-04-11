// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Share/ShareEnums.h"
#include "GOLobbyGameMode.generated.h"

UCLASS()
class GUARDIANSORDERS_API AGOLobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AGOLobbyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
