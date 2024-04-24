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
	  * ��ų�� ���۵� �� �߻��ϴ� �̺�Ʈ�� ó���մϴ�.
	  */
	void OnStartCast(UGOSkillBase* InSkillInstance);

	/**
	  * ��ų�� Ȱ��ȭ�Ǵ� ���� ����ؼ� �߻��ϴ� �̺�Ʈ�� ó���մϴ�.
	  */
	void OnUpdateCast(float DeltaTime);

	/**
	  * ��ų ����� �Ϸ�Ǿ��� �� �߻��ϴ� �̺�Ʈ�� ó���մϴ�.
	  */
	void OnFinishCast();

	/**
	  * ��ų�� �ߴܵ� �� �߻��ϴ� �̺�Ʈ�� ó���մϴ�.
	  */
	void OnInterruptCast();

private:
	//  ���� ĳ���� �������� ���� 
	bool bIsOnCasting = false;

	TObjectPtr<UGOSkillBase> CurrentSkill;

};

/*
EGOSkillType�� ���� �ٸ��� ó��
*/