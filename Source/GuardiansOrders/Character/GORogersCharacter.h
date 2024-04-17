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

protected:
	/** Q Skill */
	TObjectPtr<UGORogersSkill01> GORogersSkill01;

	/** W Skill */
	TObjectPtr<UGORogersSkill02> GORogersSkill02;

	/** E Skill */
	TObjectPtr<UGORogersSkill03> GORogersSkill03;

	/** R Skill */
	TObjectPtr<UGORogersUltimateSkill> GORogersUltimateSkill;
};