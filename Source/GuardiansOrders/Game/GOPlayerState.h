// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Share/ShareEnums.h"
#include "GOPlayerState.generated.h"

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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(class APlayerState* PlayerState) override;

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Selection")
	TSubclassOf<class AGOPlayerCharacter> SelectedCharacterClass;
};
