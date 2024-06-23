// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/GOSpellBase.h"
#include "GameData/GOGameSubsystem.h"
#include "GOSpellCastComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUARDIANSORDERS_API UGOSpellCastComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGOSpellCastComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/**
	  * ������ ���۵� �� �߻��ϴ� �̺�Ʈ�� ó���մϴ�.
	  */
	bool OnStartCast(FHeroSpellKey Key);

	/**
	  * ������ Ȱ��ȭ�Ǵ� ���� ����ؼ� �߻��ϴ� �̺�Ʈ�� ó���մϴ�.
	  */
	void OnUpdateCast(float DeltaTime);

	/**
	  * ���� ����� �Ϸ�Ǿ��� �� �߻��ϴ� �̺�Ʈ�� ó���մϴ�.
	  */
	void OnFinishCast();

	/**
	  * ������ �ߴܵ� �� �߻��ϴ� �̺�Ʈ�� ó���մϴ�.
	  */
	void OnInterruptCast();

	void UpdateCoolDownTime(float DeltaTime);
		
public:
	// ���� ��ų Get, Set
	void SetCurrentSpellByKey(FHeroSpellKey Key);
	void SetCurrentSpellKey(FHeroSpellKey Key);
	TObjectPtr<UGOSpellBase> GetCurrentSpell();
	FHeroSpellKey GetCurrentSpellKey();

private:
	//  ���� ĳ���� �������� ���� 
	bool bIsOnCasting = false;

	TObjectPtr<UGOSpellBase> CurrentSpell;
	FHeroSpellKey SpellKey;
	float CastDownTimer = 0.f;
};
