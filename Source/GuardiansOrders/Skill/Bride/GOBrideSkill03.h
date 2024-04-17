// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOBrideSkill03.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOBrideSkill03 : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOBrideSkill03();
	virtual void PostInitProperties() override;
};
