// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOKatnissSkill03.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOKatnissSkill03 : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOKatnissSkill03();
	virtual void PostInitProperties() override;
};
