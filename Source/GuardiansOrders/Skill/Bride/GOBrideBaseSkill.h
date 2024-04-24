// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOBrideBaseSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOBrideBaseSkill : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOBrideBaseSkill();
	virtual void PostInitProperties() override;
};
