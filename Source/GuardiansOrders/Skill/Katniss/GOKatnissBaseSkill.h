// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOKatnissBaseSkill.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOKatnissBaseSkill : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOKatnissBaseSkill();
	virtual void PostInitProperties() override;
};
