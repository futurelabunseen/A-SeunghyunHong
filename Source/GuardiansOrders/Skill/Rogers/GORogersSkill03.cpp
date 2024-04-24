// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersSkill03.h"

UGORogersSkill03::UGORogersSkill03()
{

}

void UGORogersSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersSkill03::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGORogersSkill03::UpdateCast(float DeltaTime)
{
}

void UGORogersSkill03::FinishCast()
{
}

void UGORogersSkill03::InterruptedCast()
{
}

void UGORogersSkill03::ActivateEffect()
{
}

bool UGORogersSkill03::IsCasting() const
{
	return false;
}

bool UGORogersSkill03::IsCastable() const
{
	return false;
}
