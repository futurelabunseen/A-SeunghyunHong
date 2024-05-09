// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideSkill01.h"

UGOBrideSkill01::UGOBrideSkill01()
{

}

void UGOBrideSkill01::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBrideSkill01::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOBrideSkill01::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOBrideSkill01::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOBrideSkill01::FinishCast()
{
	Super::FinishCast();
}

void UGOBrideSkill01::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOBrideSkill01::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOBrideSkill01::IsCasting() const
//{
//	return false;
//}
//
//bool UGOBrideSkill01::IsCastable() const
//{
//	return false;
//}
