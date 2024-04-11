// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOPlayerCharacter.h"
#include "GOBeastCharacter.generated.h"

UCLASS()
class GUARDIANSORDERS_API AGOBeastCharacter : public AGOPlayerCharacter
{
	GENERATED_BODY()

public:
	AGOBeastCharacter();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnSkillQ() override;
	virtual void OnSkillW() override;
	virtual void OnSkillE() override;
	virtual void OnSkillR() override;
};