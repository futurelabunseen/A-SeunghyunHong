// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOBrideSkill01.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOBrideSkill01 : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOBrideSkill01();
	virtual void PostInitProperties() override;
};
