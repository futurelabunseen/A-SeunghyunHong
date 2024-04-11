// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOHeroAnimInstance.h"
#include "GOBeastAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOBeastAnimInstance : public UGOHeroAnimInstance
{
	GENERATED_BODY()
	
public:
	UGOBeastAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
