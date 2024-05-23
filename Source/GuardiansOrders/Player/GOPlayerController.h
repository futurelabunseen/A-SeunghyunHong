// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Share/ShareEnums.h"
#include "GOPlayerController.generated.h"

class UGOHUDWidget;
class UGOSkillSetBarWidget;

UCLASS()
class GUARDIANSORDERS_API AGOPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGOPlayerController();

protected:
	virtual void PostInitializeComponents() override;
	
	/**
	���� Ŭ���̾�Ʈ���� ��Ʈ��ũ�� �ʱ�ȭ�� �ʿ��� ������ ���޹��� ���� 
	��� �������Ǹ� (Ŭ���̾�Ʈ������) ȣ��Ǵ� �Լ��Դϴ�.
	*/
	virtual void PostNetInit() override;
	
	virtual void PostSeamlessTravel() override;

	virtual void BeginPlay() override;

	/**
	������ �� �߻��ϴ� �̺�Ʈ �Լ��Դϴ�.
	*/
	virtual void OnPossess(APawn* InPawn) override;

// HUD Section
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HUD)
	TSubclassOf<UGOHUDWidget> GOHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=HUD)
	TObjectPtr<UGOHUDWidget> GOHUDWidget;

	void SetHUDScore(float Score);

// ======== SkillSetBar UI ======== 
protected:
	// SkillSetBar Widget �ν��Ͻ�
	UPROPERTY()
	TObjectPtr<UGOSkillSetBarWidget> SkillSetBarWidget;

public:
	void InitializeSkills();


// ======== SpawnAndPossess ======== 
public:
	// New function to spawn and possess a character
	void SpawnAndPossessCharacter();

private:
	// Helper function to get the selected hero type from the player state
	EHeroType GetSelectedHero();

};
