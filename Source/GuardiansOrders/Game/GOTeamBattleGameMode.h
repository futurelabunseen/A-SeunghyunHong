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
	AGOTeamBattleGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void PostSeamlessTravel() override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	virtual void StartPlay() override;
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);
	virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn);

protected:
	virtual void HandleMatchHasStarted() override;
	virtual void DefaultRoundTimer() override;
	void FinishMatch();

};
