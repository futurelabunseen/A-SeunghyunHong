// Fill out your copyright notice in the Description page of Project Settings.


#include "GOBeastAnimInstance.h"

UGOBeastAnimInstance::UGOBeastAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UGOBeastAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UGOBeastAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}