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
	  * 스펠이 시작될 때 발생하는 이벤트를 처리합니다.
	  */
	bool OnStartCast(FHeroSpellKey Key);

	/**
	  * 스펠이 활성화되는 동안 계속해서 발생하는 이벤트를 처리합니다.
	  */
	void OnUpdateCast(float DeltaTime);

	/**
	  * 스펠 사용이 완료되었을 때 발생하는 이벤트를 처리합니다.
	  */
	void OnFinishCast();

	/**
	  * 스펠이 중단될 때 발생하는 이벤트를 처리합니다.
	  */
	void OnInterruptCast();

	void UpdateCoolDownTime(float DeltaTime);
		
public:
	// 현재 스킬 Get, Set
	void SetCurrentSpellByKey(FHeroSpellKey Key);
	void SetCurrentSpellKey(FHeroSpellKey Key);
	TObjectPtr<UGOSpellBase> GetCurrentSpell();
	FHeroSpellKey GetCurrentSpellKey();

private:
	//  현재 캐스팅 중인지의 여부 
	bool bIsOnCasting = false;

	TObjectPtr<UGOSpellBase> CurrentSpell;
	FHeroSpellKey SpellKey;
	float CastDownTimer = 0.f;
};
