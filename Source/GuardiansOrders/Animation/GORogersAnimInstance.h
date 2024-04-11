// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOHeroAnimInstance.h"
#include "GORogersAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGORogersAnimInstance : public UGOHeroAnimInstance
{
	GENERATED_BODY()
	
public:
	UGORogersAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
