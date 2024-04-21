// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOBrideUltimateSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOBrideUltimateSkill : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOBrideUltimateSkill();
	virtual void PostInitProperties() override;
};
