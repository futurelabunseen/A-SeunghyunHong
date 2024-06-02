// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Projectile/GOProjectileSkillBase.h"
#include "GOKatnissUltimateSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOKatnissUltimateSkill : public UGOProjectileSkillBase
{
	GENERATED_BODY()
	
public:
	UGOKatnissUltimateSkill();
	virtual void PostInitProperties() override;

public:
	virtual void StartCast() override;
	virtual void UpdateCast(float DeltaTime) override;
	virtual void ActivateSkill() override;
	virtual void FinishCast() override;
	virtual void InterruptedCast() override;
	virtual void ActivateEffect() override;
	//virtual bool IsCasting() const override;
	//virtual bool IsCastable() const override;

private:
	//bool bIsCastable = false;
	//bool bIsOnCasting = false;
};
