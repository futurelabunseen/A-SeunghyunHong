// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOHeroAnimInstance.h"
#include "GOBrideAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOBrideAnimInstance : public UGOHeroAnimInstance
{
	GENERATED_BODY()
	
public:
	UGOBrideAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
