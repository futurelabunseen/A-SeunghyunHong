// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Share/ShareEnums.h"
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

	// 추가해줌
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCommonUserWidget> HeroSelectionWidgetClass;



	UPROPERTY(ReplicatedUsing = OnRep_ShowHeroSelectionWidget)
	bool bShowHeroSelectionWidget;

	UPROPERTY(ReplicatedUsing = OnRep_ShowHeroSelectionWidget)
	bool bShowHeroSelectionWidget2;

public:
	UFUNCTION()
	void OnRep_ShowHeroSelectionWidget();

	// 정리
	UFUNCTION()
	void OnRep_HeroSelectionWidget();

	UFUNCTION(BlueprintCallable)
	void ShowHeroSelectionWidget();

	UFUNCTION()
	void OnGamePlayerReadyNotified();

private:
	void DisplayHeroSelectionWidget(APlayerController* PlayerController);
};
