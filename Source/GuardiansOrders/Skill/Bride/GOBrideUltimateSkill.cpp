// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideUltimateSkill.h"

UGOBrideUltimateSkill::UGOBrideUltimateSkill()
{

}

void UGOBrideUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBrideUltimateSkill::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOBrideUltimateSkill::UpdateCast(float DeltaTime)
{
}

void UGOBrideUltimateSkill::FinishCast()
{
}

void UGOBrideUltimateSkill::InterruptedCast()
{
}

void UGOBrideUltimateSkill::ActivateEffect()
{
}

bool UGOBrideUltimateSkill::IsCasting() const
{
	return false;
}

bool UGOBrideUltimateSkill::IsCastable() const
{
	return false;
}
