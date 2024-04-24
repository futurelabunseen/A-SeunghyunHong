// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideSkill03.h"

UGOBrideSkill03::UGOBrideSkill03()
{

}

void UGOBrideSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBrideSkill03::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOBrideSkill03::UpdateCast(float DeltaTime)
{
}

void UGOBrideSkill03::FinishCast()
{
}

void UGOBrideSkill03::InterruptedCast()
{
}

void UGOBrideSkill03::ActivateEffect()
{
}

bool UGOBrideSkill03::IsCasting() const
{
	return false;
}

bool UGOBrideSkill03::IsCastable() const
{
	return false;
}
