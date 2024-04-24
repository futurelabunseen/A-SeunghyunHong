// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersUltimateSkill.h"

UGORogersUltimateSkill::UGORogersUltimateSkill()
{

}

void UGORogersUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersUltimateSkill::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGORogersUltimateSkill::UpdateCast(float DeltaTime)
{
}

void UGORogersUltimateSkill::FinishCast()
{
}

void UGORogersUltimateSkill::InterruptedCast()
{
}

void UGORogersUltimateSkill::ActivateEffect()
{
}

bool UGORogersUltimateSkill::IsCasting() const
{
	return false;
}

bool UGORogersUltimateSkill::IsCastable() const
{
	return false;
}
