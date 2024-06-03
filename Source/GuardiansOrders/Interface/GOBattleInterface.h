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
	* ĳ������ ������ ����� ���ؼ�, �������� ��ŸƮ �����ǿ� ���� Ʈ������ ���� �������� �Լ��Դϴ�.
	*/
	virtual FTransform GetRandomStartTransform() const = 0;

	virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn) = 0;

	//virtual FVector GetBattleSocketLocation() = 0;
};
