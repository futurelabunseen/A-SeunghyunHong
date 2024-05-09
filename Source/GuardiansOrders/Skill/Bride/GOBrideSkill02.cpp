// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideSkill02.h"

UGOBrideSkill02::UGOBrideSkill02()
{

}

void UGOBrideSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBrideSkill02::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOBrideSkill02::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOBrideSkill02::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOBrideSkill02::FinishCast()
{
	Super::FinishCast();
}

void UGOBrideSkill02::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOBrideSkill02::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOBrideSkill02::IsCasting() const
//{
//	return false;
//}
//
//bool UGOBrideSkill02::IsCastable() const
//{
//	return false;
//}
