// Fill out your copyright notice in the Description page of Project Settings.


#include "GOBrideAnimInstance.h"

UGOBrideAnimInstance::UGOBrideAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UGOBrideAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UGOBrideAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}