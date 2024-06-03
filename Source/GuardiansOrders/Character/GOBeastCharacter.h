// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOPlayerCharacter.h"
#include "Skill/Beast/GOBeastSkill01.h"
#include "Skill/Beast/GOBeastSkill02.h"
#include "Skill/Beast/GOBeastSkill03.h"
#include "Skill/Beast/GOBeastUltimateSkill.h"
#include "GOBeastCharacter.generated.h"

UCLASS()
class GUARDIANSORDERS_API AGOBeastCharacter : public AGOPlayerCharacter
{
	GENERATED_BODY()

public:
	AGOBeastCharacter(const FObjectInitializer& ObjectInitializer);
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
};