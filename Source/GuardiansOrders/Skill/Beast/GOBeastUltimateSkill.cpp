// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastUltimateSkill.h"

UGOBeastUltimateSkill::UGOBeastUltimateSkill()
{

}

void UGOBeastUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBeastUltimateSkill::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOBeastUltimateSkill::UpdateCast(float DeltaTime)
{
}

void UGOBeastUltimateSkill::FinishCast()
{
}

void UGOBeastUltimateSkill::InterruptedCast()
{
}

void UGOBeastUltimateSkill::ActivateEffect()
{
}

bool UGOBeastUltimateSkill::IsCasting() const
{
	return false;
}

bool UGOBeastUltimateSkill::IsCastable() const
{
	return false;
}
