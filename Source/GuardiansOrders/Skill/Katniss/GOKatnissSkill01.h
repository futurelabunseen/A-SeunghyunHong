// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOKatnissSkill01.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOKatnissSkill01 : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOKatnissSkill01();
	virtual void PostInitProperties() override;
};
