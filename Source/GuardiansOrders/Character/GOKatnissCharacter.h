// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GOPlayerCharacter.h"
#include "Skill/Katniss/GOKatnissSkill01.h"
#include "Skill/Katniss/GOKatnissSkill02.h"
#include "Skill/Katniss/GOKatnissSkill03.h"
#include "Skill/Katniss/GOKatnissUltimateSkill.h"
#include "GOKatnissCharacter.generated.h"

UCLASS()
class GUARDIANSORDERS_API AGOKatnissCharacter : public AGOPlayerCharacter
{
	GENERATED_BODY()

public:
	AGOKatnissCharacter();
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
	/** Q Skill */
	TObjectPtr<UGOKatnissSkill01> GOKatnissSkill01;

	/** W Skill */
	TObjectPtr<UGOKatnissSkill02> GOKatnissSkill02;

	/** E Skill */
	TObjectPtr<UGOKatnissSkill03> GOKatnissSkill03;

	/** R Skill */
	TObjectPtr<UGOKatnissUltimateSkill> GOKatnissUltimateSkill;
};

