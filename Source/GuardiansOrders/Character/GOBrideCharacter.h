// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOPlayerCharacter.h"
#include "Skill/Bride/GOBrideSkill01.h"
#include "Skill/Bride/GOBrideSkill02.h"
#include "Skill/Bride/GOBrideSkill03.h"
#include "Skill/Bride/GOBrideUltimateSkill.h"
#include "GOBrideCharacter.generated.h"

UCLASS()
class GUARDIANSORDERS_API AGOBrideCharacter : public AGOPlayerCharacter
{
	GENERATED_BODY()
	
public:
	AGOBrideCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnBaseSkill() override;
	virtual void OnSkillQ() override;
	virtual void OnSkillW() override;
	virtual void OnSkillE() override;
	virtual void OnSkillR() override;

protected:
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

// Skill Section
protected:
	/** Q Skill */
	TObjectPtr<UGOBrideSkill01> UGOBrideSkill01;

	/** W Skill */
	TObjectPtr<UGOBrideSkill02> UGOBrideSkill02;

	/** E Skill */
	TObjectPtr<UGOBrideSkill03> UGOBrideSkill03;

	/** R Skill */
	TObjectPtr<UGOBrideUltimateSkill> UGOBrideUltimateSkill;
};
