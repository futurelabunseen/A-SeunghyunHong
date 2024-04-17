// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOKatnissSkill02.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOKatnissSkill02 : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOKatnissSkill02();
	virtual void PostInitProperties() override;
};
