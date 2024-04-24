// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersSkill01.h"

UGORogersSkill01::UGORogersSkill01()
{

}

void UGORogersSkill01::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersSkill01::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGORogersSkill01::FinishCast()
{
}

void UGORogersSkill01::InterruptedCast()
{
}

void UGORogersSkill01::ActivateEffect()
{
}

bool UGORogersSkill01::IsCasting() const
{
	return false;
}

bool UGORogersSkill01::IsCastable() const
{
	return GetCoolDownTimer() <= 0;
}
