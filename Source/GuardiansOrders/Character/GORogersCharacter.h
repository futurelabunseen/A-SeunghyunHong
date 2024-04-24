// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GOPlayerCharacter.h"
#include "Skill/Rogers/GORogersSkill01.h"
#include "Skill/Rogers/GORogersSkill02.h"
#include "Skill/Rogers/GORogersSkill03.h"
#include "Skill/Rogers/GORogersUltimateSkill.h"
#include "GORogersCharacter.generated.h"

UCLASS()
class GUARDIANSORDERS_API AGORogersCharacter : public AGOPlayerCharacter
{
	GENERATED_BODY()

public:
	AGORogersCharacter();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnSkillQ() override;
	virtual void OnSkillW() override;
	virtual void OnSkillE() override;
	virtual void OnSkillR() override;

// Skill Section
protected:

// ======== IPlaySkillAnimInterface ========
	virtual UGOSkillCastComponent* GetSkillCastComponent()
	{
		return SkillCastComponent;
	}

	virtual void PlaySkillAnim(UGOSkillBase* CurrentSkill) override;

};