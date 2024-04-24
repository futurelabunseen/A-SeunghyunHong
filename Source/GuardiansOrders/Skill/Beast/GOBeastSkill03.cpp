// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastSkill03.h"

UGOBeastSkill03::UGOBeastSkill03()
{

}

void UGOBeastSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBeastSkill03::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOBeastSkill03::UpdateCast(float DeltaTime)
{
}

void UGOBeastSkill03::FinishCast()
{
}

void UGOBeastSkill03::InterruptedCast()
{
}

void UGOBeastSkill03::ActivateEffect()
{
}

bool UGOBeastSkill03::IsCasting() const
{
	return false;
}

bool UGOBeastSkill03::IsCastable() const
{
	return false;
}
