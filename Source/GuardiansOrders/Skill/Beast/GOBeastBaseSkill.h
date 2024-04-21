// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOBeastBaseSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOBeastBaseSkill : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOBeastBaseSkill();
	virtual void PostInitProperties() override;
};
