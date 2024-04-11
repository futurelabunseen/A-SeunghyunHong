// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOPlayerCharacter.h"
#include "GOBrideCharacter.generated.h"

UCLASS()
class GUARDIANSORDERS_API AGOBrideCharacter : public AGOPlayerCharacter
{
	GENERATED_BODY()
	
public:
	AGOBrideCharacter();
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
