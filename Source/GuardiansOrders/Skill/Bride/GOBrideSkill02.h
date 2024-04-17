// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOBrideSkill02.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOBrideSkill02 : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOBrideSkill02();
	virtual void PostInitProperties() override;
};
