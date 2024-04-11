// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GOCharacterHUDInterface.generated.h"

class UGOHUDWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGOCharacterHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GUARDIANSORDERS_API IGOCharacterHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupHUDWidget(UGOHUDWidget* InHUDWidget) = 0;
};
