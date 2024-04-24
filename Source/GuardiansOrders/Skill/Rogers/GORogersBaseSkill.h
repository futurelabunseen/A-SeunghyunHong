// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GORogersBaseSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGORogersBaseSkill : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGORogersBaseSkill();
	virtual void PostInitProperties() override;

};
