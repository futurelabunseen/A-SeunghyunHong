// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GOCharacterBase.h"
#include "GONonPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API AGONonPlayerCharacter : public AGOCharacterBase
{
	GENERATED_BODY()
	
public:
	AGONonPlayerCharacter();

protected:
	void SetDead() override;
};
