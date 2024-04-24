// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissUltimateSkill.h"

UGOKatnissUltimateSkill::UGOKatnissUltimateSkill()
{

}

void UGOKatnissUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissUltimateSkill::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOKatnissUltimateSkill::UpdateCast(float DeltaTime)
{
}

void UGOKatnissUltimateSkill::FinishCast()
{
}

void UGOKatnissUltimateSkill::InterruptedCast()
{
}

void UGOKatnissUltimateSkill::ActivateEffect()
{
}

bool UGOKatnissUltimateSkill::IsCasting() const
{
	return false;
}

bool UGOKatnissUltimateSkill::IsCastable() const
{
	return false;
}
