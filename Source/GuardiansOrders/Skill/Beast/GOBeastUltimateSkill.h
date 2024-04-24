// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOBeastUltimateSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOBeastUltimateSkill : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOBeastUltimateSkill();
	virtual void PostInitProperties() override;

public:
	virtual void StartCast() override;
	virtual void UpdateCast(float DeltaTime) override;
	virtual void FinishCast() override;
	virtual void InterruptedCast() override;
	virtual void ActivateEffect() override;
	virtual bool IsCasting() const override;
	virtual bool IsCastable() const override;

private:
	//bool bIsCastable = false;
	//bool bIsOnCasting = false;
};
