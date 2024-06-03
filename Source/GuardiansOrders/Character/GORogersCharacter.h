// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GOPlayerCharacter.h"
#include "GORogersCharacter.generated.h"

UCLASS()
class GUARDIANSORDERS_API AGORogersCharacter : public AGOPlayerCharacter
{
	GENERATED_BODY()

public:
	AGORogersCharacter(const FObjectInitializer& ObjectInitializer);
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

// ======== IPlaySkillAnimInterface ========
//	virtual UGOSkillCastComponent* GetSkillCastComponent()
//	{
//		return SkillCastComponent;
//	}
//
//	virtual void PlaySkillAnim(UGOSkillBase* CurrentSkill) override;
//
//

};