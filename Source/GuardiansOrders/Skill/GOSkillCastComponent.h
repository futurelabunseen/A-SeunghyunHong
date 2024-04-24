// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/GOSkillBase.h"
#include "GOSkillCastComponent.generated.h"


class GOSkillCastComponent;

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

private:
	//  현재 캐스팅 중인지의 여부 
	bool bIsOnCasting = false;

	TObjectPtr<UGOSkillBase> CurrentSkill;

};

/*
EGOSkillType에 따라 다르게 처리
*/