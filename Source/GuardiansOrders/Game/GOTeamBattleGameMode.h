// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOBattleGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "GOTeamBattleGameMode.generated.h"

namespace MatchState
{
	extern GUARDIANSORDERS_API const FName Cooldown;
}
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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime);
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);
	virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn);

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime  = 5.f; 

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 5.f; // at the Ready State

	float LevelStartingTime = 0.f;

	float CountdownTime  = 0.f;

protected:
	virtual void OnMatchStateSet() override;
	virtual void HandleMatchHasStarted() override;
	virtual void DefaultRoundTimer() override;
	void FinishMatch();

};
