// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastSkill01.h"

UGOBeastSkill01::UGOBeastSkill01()
{

}

void UGOBeastSkill01::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBeastSkill01::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOBeastSkill01::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOBeastSkill01::FinishCast()
{
	Super::FinishCast();
}

void UGOBeastSkill01::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOBeastSkill01::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOBeastSkill01::IsCasting() const
//{
//	return false;
//}
//
//bool UGOBeastSkill01::IsCastable() const
//{
//	return false;
//}
