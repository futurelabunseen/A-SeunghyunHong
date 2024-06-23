// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GORogersSkill02.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGORogersSkill02 : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGORogersSkill02();
	virtual void PostInitProperties() override;

public:
	virtual void StartCast() override;
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateCast(float DeltaTime) override;
	virtual void ActivateSkill() override;
	virtual void FinishCast() override;
	virtual void InterruptedCast() override;
	virtual void ActivateEffect() override;
	//virtual bool IsCasting() const override;
	//virtual bool IsCastable() const override;

	//bool bIsCastable = false;
	//bool bIsOnCasting = false;

private:
	void ShieldSkillFinished();
};
