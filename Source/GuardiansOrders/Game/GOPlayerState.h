// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Share/ShareEnums.h"
#include "GOPlayerState.generated.h"

/**
 * ������ ��� Ŭ���̾�Ʈ�� �����մϴ�.
 * �ڽ�, �ٸ� �÷��̾��� ���¸� �ľ��� �� �ֽ��ϴ�.
 * ������ ���ھ� �� �ٸ� ������ ������ �÷��̾� ������ �����ϴµ� �����մϴ�.
 * ���ھ�, �÷��̾� �̸�, �÷��̾� ID ���� ������ �� �ֽ��ϴ�.
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
