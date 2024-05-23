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
	AGOPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(class APlayerState* PlayerState) override;

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Selection")
	TSubclassOf<AGOPlayerCharacter> SelectedCharacterClass;

	// �÷��̾ ������ ���� ����
	UPROPERTY(Replicated)
	FHeroSelectionInfo SelectedHero;

	UPROPERTY(Replicated)
	bool bIsReady; // Ready ���¸� �����ϴ� ���� �߰�

	// �߰�
	// 
	//// ĳ���� ���� �Ϸ� ����
	//UPROPERTY(ReplicatedUsing = OnRep_CharacterSelected, BlueprintReadOnly, Category = "Character Selection")
	//bool bCharacterSelected;

	//UFUNCTION()
	//void OnRep_CharacterSelected();

	//// ĳ���� ���� �Լ�
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
	// �� ���� (Red, Blue)
	UPROPERTY(ReplicatedUsing = OnRep_Team)
	ETeamType Team = ETeamType::ET_NoTeam;
};
