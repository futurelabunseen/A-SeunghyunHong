// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "GOCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
    UFUNCTION(exec)
    void ShowDebugLog();

    UFUNCTION(exec)
    void AuthorityStatus();
};
