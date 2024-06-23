// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSpellBase.h"
#include "GOHealSpell.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOHealSpell : public UGOSpellBase
{
	GENERATED_BODY()

public:
	UGOHealSpell();

public:
	virtual void StartCast() override;
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateCast(float DeltaTime) override;
	virtual void FinishCast() override;
	virtual void InterruptedCast() override;
	virtual void ActivateEffect() override;
};
