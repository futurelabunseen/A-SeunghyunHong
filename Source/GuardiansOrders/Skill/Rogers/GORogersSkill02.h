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

};
