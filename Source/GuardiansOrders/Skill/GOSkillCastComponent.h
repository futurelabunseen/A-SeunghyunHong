// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/GOSkillBase.h"
#include "GOSkillCastComponent.generated.h"

class GOSkillCastComponent;
// class AGOCharacterBase;

/*
  * 스킬이 장전되고 발사되는 발사대 역할로, 스킬 발동과 관련된 흐름을 제어합니다.
  * EGOSkillType에 따라 다르게 처리합니다.
  * 이 컴포넌트가 스킬의 사용을 시작, 업데이트, 완료, 중단하는 것을 캐릭터 또는 플레이어의 행동과 연결시키는 역할을 합니다. 
  */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUARDIANSORDERS_API UGOSkillCastComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGOSkillCastComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/**
	  * 스킬이 시작될 때 발생하는 이벤트를 처리합니다.
	  */
	void OnStartCast(UGOSkillBase* InSkillInstance);

	/**
	  * 스킬이 활성화되는 동안 계속해서 발생하는 이벤트를 처리합니다.
	  */
	void OnUpdateCast(float DeltaTime);
	
	/**
	  * 스킬 사용이 완료되었을 때 발생하는 이벤트를 처리합니다.
	  */
	void OnFinishCast();
	
	/**
	  * 스킬이 중단될 때 발생하는 이벤트를 처리합니다.
	  */
	void OnInterruptCast();

protected:
	/**
	  * 타겟 스킬 처리 로직
	  */
	void HandleTargetSkill(UGOSkillBase* SkillInstance);
	
	/**
	  * 자동 타겟 스킬 처리 로직
	  */
	void HandleAutoTargetSkill(UGOSkillBase* SkillInstance);
	
	/**
	  * 방향성 스킬 처리 로직
	  */
	void HandleDirectionalSkill(UGOSkillBase* SkillInstance);
	
	/**
	  * 범위 스킬 처리 로직
	  */
	void HandleAreaOfEffectSkill(UGOSkillBase* SkillInstance);

private:
	//  현재 캐스팅 중인지의 여부 
	bool bIsOnCasting = false; 

	TObjectPtr<UGOSkillBase> CurrentSkill;
};
