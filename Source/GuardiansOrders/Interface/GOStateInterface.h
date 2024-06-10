// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GOStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGOStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GUARDIANSORDERS_API IGOStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool GetIsDead() = 0;
	virtual bool GetIsStunned() = 0;
};
