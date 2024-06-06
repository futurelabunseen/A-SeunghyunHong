// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Projectile/GOProjectileSkillBase.h"
#include "GOKatnissSkill03.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOKatnissSkill03 : public UGOProjectileSkillBase
{
	GENERATED_BODY()
	
public:
	UGOKatnissSkill03();
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
	void SpawnSequentialProjectiles();
	void SpawnProjectile(FVector Location, FRotator Rotation);

	int32 ProjectilesFired;
	FTimerHandle ProjectileTimerHandle;
	//bool bIsCastable = false;
	//bool bIsOnCasting = false;
};
