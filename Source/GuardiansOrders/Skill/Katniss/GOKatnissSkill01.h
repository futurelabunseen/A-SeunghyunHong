// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Projectile/GOProjectileSkillBase.h"
#include "GOKatnissSkill01.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOKatnissSkill01 : public UGOProjectileSkillBase
{
	GENERATED_BODY()
	
public:
	UGOKatnissSkill01();
	virtual void PostInitProperties() override;

public:
	virtual void StartCast() override;
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateCast(float DeltaTime) override;
	virtual void ActivateSkill() override;
	virtual void FinishCast() override;
	virtual void InterruptedCast() override;
	virtual void ActivateEffect() override;
	//virtual bool IsCasting() const override;
	//virtual bool IsCastable() const override;

private:
	void StartArrowRain();
	void EndArrowRain();

	FVector SkillLocation;

	FTimerHandle ArrowRainTimerHandle;
	FTimerHandle DamageTimerHandle;

	const float Duration = 5.0f;
	const float SpawnInterval = 10.f; // Time interval between each projectile spawn
	const int32 NumProjectilesPerSpawn = 10; // Number of projectiles to spawn each interval

	//bool bIsCastable = false;
	//bool bIsOnCasting = false;
};
