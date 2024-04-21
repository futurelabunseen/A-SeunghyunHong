// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GORogersUltimateSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGORogersUltimateSkill : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGORogersUltimateSkill();
	virtual void PostInitProperties() override;
};
