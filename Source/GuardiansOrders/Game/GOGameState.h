// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Share/ShareEnums.h"
#include "GOPlayerState.h"
#include "GOGameState.generated.h"

/**
 * 서버와 모든 클라이언트에 존재합니다.
 * 클라이언트는 게임 스테이트를 사용해 현재 게임의 상태를 파악할 수 있습니다.
 * 팀 스코어, 현재 월드의 시간, 플레이어 스테이트의 배열 등을 관리할 수 있습니다.
 */




UCLASS()
class GUARDIANSORDERS_API AGOGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AGOGameState();
	/** 
	GameMode가 StartPlay를 호출하면 이 함수를 통해 월드에 있는 모든 액터가 BeginPlay를 실행하고 게임을 시작하도록 명령합니다.
	*/
	virtual void HandleBeginPlay() override;

	/**
	클라이언트에 복제된 GameState에 의해서 호출되는 함수입니다.
	bReplicatedHasBegunPlay 속성이 변경되었을 때 호출되고, 
	클라이언트에 복제된 플레이어 컨트롤러의 BeginPlay가 호출됩니다.
	*/
	virtual void OnRep_ReplicatedHasBegunPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Transient, Replicated)
	int32 RemainingTime;

	int32 MatchPlayTime = 20;
	
	int32 ShowResultWaitingTime = 5;
	//const int32 ReadyForTravelTime = 5;

	// 추가해줌
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCommonUserWidget> HeroSelectionWidgetClass;

	// 정리
	UPROPERTY(ReplicatedUsing = OnRep_HeroSelectionWidget)
	bool bShowHeroSelectionWidget;

	// OnRep 함수: Replication이 발생했을 때 클라이언트에서 호출된다.
	UFUNCTION()
	void OnRep_CharacterSelected();

	UPROPERTY(ReplicatedUsing = OnRep_CountDownForTravelReadyTime)
	int32 RemainingReadyTravelTime;

	UFUNCTION()
	void OnRep_CountDownForTravelReadyTime();

	// Red팀과 Blue팀의 영웅 정보를 담을 배열
	UPROPERTY(ReplicatedUsing = OnRep_CharacterSelected)
	TArray<FHeroSelectionInfo> RedTeamHeroes;

	UPROPERTY(ReplicatedUsing = OnRep_CharacterSelected)
	TArray<FHeroSelectionInfo> BlueTeamHeroes;


	void RedTeamScores();
	void BlueTeamScores();

	TArray<AGOPlayerState*> RedTeam;
	TArray<AGOPlayerState*> BlueTeam;

	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore)
	int32 RedTeamScore = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore)
	int32 BlueTeamScore = 0.f;

	UFUNCTION()
	void OnRep_RedTeamScore();

	UFUNCTION()
	void OnRep_BlueTeamScore();
	
	/** 
	 * Top Score
	 */
	void UpdateTopscore(AGOPlayerState* ScoringPlayer);

	// 복수일 수 있다
	UPROPERTY(Replicated)
	TArray<AGOPlayerState*> TopScoringPlayers;

private:
	float TopScore = 0.f;

public:
	UFUNCTION()
	void OnRep_HeroSelectionWidget();

	UFUNCTION(BlueprintCallable)
	void ShowHeroSelectionWidget();

	UFUNCTION()
	void OnGamePlayerReadyNotified();

	//// 모든 플레이어가 캐릭터를 선택했는지 확인//바꿔야함
	//void CheckAllPlayersSelected();
	
	bool AreAllPlayersReady(); // 모든 플레이어의 Ready 상태를 확인하는 함수

private:
	void DisplayHeroSelectionWidget(APlayerController* PlayerController);


};
