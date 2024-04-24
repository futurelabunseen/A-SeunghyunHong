// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOBeastSkill01.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOBeastSkill01 : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOBeastSkill01();
	virtual void PostInitProperties() override;
};
