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
};
