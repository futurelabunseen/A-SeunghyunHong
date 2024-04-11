// Fill out your copyright notice in the Description page of Project Settings.


#include "GORogersAnimInstance.h"

UGORogersAnimInstance::UGORogersAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UGORogersAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UGORogersAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}