// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interface/GOBattleInterface.h"
#include "Share/ShareEnums.h"
#include "GOBattleGameMode.generated.h"

class APlayerStart;


/**
 * 오직 서버에만 존재합니다. 
 * 현재 게임에 참여하고 있는 플레이어의 수
 * 플레이어가 게임에 입장하는 방법과 스폰, 리스폰 규칙
 * 대미지 처리, 스코어에 관련된 로직 등을 처리합니다.
 */

UCLASS()
class GUARDIANSORDERS_API AGOBattleGameMode : public AGameMode, public IGOBattleInterface
{
	GENERATED_BODY()
	
public:
	AGOBattleGameMode();

	/**
	 * (리슨 서버 자신을 포함한) 클라이언트의 접속 요청을 처리하는 함수입니다.접속을 거부할 수도 있습니다.
	 * ErrorMessage 인자에 아무 값도 작성되어있지 않다면 접속이 통과됩니다.
	 */
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	/**
	 * 접속을 허용한 클라이언트에 대응하는 플레이어 컨트롤러를 만드는 함수입니다.
	 */
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	/**
	 * 플레이어의 입장을 위해 플레이어에 필요한 기본 설정을 모두 마무리하는 함수입니다. (빙의할 캐릭터 세팅 등)
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/**
	 * 게임 시작 조건에 부합하면 공식적으로 게임의 시작을 지시하는 함수입니다.
	 */
	virtual void StartPlay() override;

protected:
	virtual void PostInitializeComponents() override;

	/**
	 * 1초에 한 번씩 수행됩니다.
	 */
	virtual void DefaultRoundTimer(); 
	
	/**
	 * 현재 매치가 종료되면 호출될 함수입니다.
	 */
	void  FinishMatch(); 

	FTimerHandle RoundTimerHandle;
public:
	virtual FTransform GetRandomStartTransform() const;
	virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn);

protected:
	TArray<TObjectPtr<APlayerStart>> PlayerStartArray;

private:
	void SpawnPlayerCharacter(APlayerController* NewPlayer, EHeroType HeroType);
};
