// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Skill/GOSkillCastComponent.h"
#include "GOPlaySkillAnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGOPlaySkillAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GUARDIANSORDERS_API IGOPlaySkillAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UGOSkillCastComponent* GetSkillCastComponent() = 0;
	// virtual void PlaySkillAnim() = 0;
	virtual void PlaySkillAnim(UGOSkillBase* CurrentSkill) = 0;
	virtual void PlaySkillAnimByKey(FHeroSkillKey Key) = 0;
	virtual void ActivateSkillByKey(FHeroSkillKey Key) = 0;
	//virtual void ActivateSkill(ASkillSlot* CurrentSkillSlot) = 0;
};
