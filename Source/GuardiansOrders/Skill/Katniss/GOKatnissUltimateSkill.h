// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "GOKatnissUltimateSkill.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOKatnissUltimateSkill : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOKatnissUltimateSkill();
	virtual void PostInitProperties() override;
};
