// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GOBattleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGOBattleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GUARDIANSORDERS_API IGOBattleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	* 캐릭터의 리스폰 기능을 위해서, 랜덤으로 스타트 포지션에 대한 트랜스폼 값을 가져오는 함수입니다.
	*/
	virtual FTransform GetRandomStartTransform() const = 0;

	virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn) = 0;

	//virtual FVector GetBattleSocketLocation() = 0;
};
