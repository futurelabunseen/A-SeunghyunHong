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
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOBrideSkill01::UpdateCast(float DeltaTime)
{
}

void UGOBrideSkill01::FinishCast()
{
}

void UGOBrideSkill01::InterruptedCast()
{
}

void UGOBrideSkill01::ActivateEffect()
{
}

bool UGOBrideSkill01::IsCasting() const
{
	return false;
}

bool UGOBrideSkill01::IsCastable() const
{
	return false;
}
