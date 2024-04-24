// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOBeastSkill02.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOBeastSkill02 : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOBeastSkill02();
	virtual void PostInitProperties() override;
};
