// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOBattleGameMode.h"
#include "GOTeamBattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API AGOTeamBattleGameMode : public AGOBattleGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Existing) override;
	virtual void PostSeamlessTravel() override;

protected:
	virtual void HandleMatchHasStarted() override;
};
