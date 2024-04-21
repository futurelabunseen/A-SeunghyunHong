// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GORogersSkill03.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGORogersSkill03 : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGORogersSkill03();
	virtual void PostInitProperties() override;

};
