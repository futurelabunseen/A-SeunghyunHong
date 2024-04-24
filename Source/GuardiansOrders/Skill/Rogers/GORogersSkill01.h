// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GORogersSkill01.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGORogersSkill01 : public UGOSkillBase
{
	GENERATED_BODY()
	
public: 
	UGORogersSkill01();
	virtual void PostInitProperties() override;

};
