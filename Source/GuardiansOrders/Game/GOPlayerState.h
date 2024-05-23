// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Share/ShareEnums.h"
#include "Share/EGOTeam.h"
#include "GOPlayerState.generated.h"

class AGOPlayerCharacter;
class AGOPlayerController;
/**
 * 서버와 모든 클라이언트에 존재합니다.
 * 자신, 다른 플레이어의 상태를 파악할 수 있습니다.
 * 개인의 스코어 및 다른 사람들과 공유할 플레이어 정보를 관리하는데 유용합니다.
 * 스코어, 플레이어 이름, 플레이어 ID 등을 관리할 수 있습니다.
 * Score, PlayerId, Ping ...
 */
UCLASS()
class GUARDIANSORDERS_API AGOPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AGOPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(class APlayerState* PlayerState) override;

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Selection")
	TSubclassOf<AGOPlayerCharacter> SelectedCharacterClass;

	// 플레이어가 선택한 영웅 정보
	UPROPERTY(Replicated)
	FHeroSelectionInfo SelectedHero;

	UPROPERTY(Replicated)
	bool bIsReady; // Ready 상태를 저장하는 변수 추가

	// 추가
	// 
	//// 캐릭터 선택 완료 여부
	//UPROPERTY(ReplicatedUsing = OnRep_CharacterSelected, BlueprintReadOnly, Category = "Character Selection")
	//bool bCharacterSelected;

	//UFUNCTION()
	//void OnRep_CharacterSelected();

	//// 캐릭터 선택 함수
	//void SelectCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass);

	/**
	 * Score
	 */

public:
	virtual void OnRep_Score() override;
	void AddToScore(float ScoreAmount);

private:
	AGOPlayerCharacter* Character;
	AGOPlayerController* Controller;

	/**
	 * Team
	 */
public:
	FORCEINLINE ETeamType GetTeamType() const { return Team; }
	void SetTeam(ETeamType TeamToSet);

	UFUNCTION()
	void OnRep_Team();

private:
	// 팀 정보 (Red, Blue)
	UPROPERTY(ReplicatedUsing = OnRep_Team)
	ETeamType Team = ETeamType::ET_NoTeam;
};
