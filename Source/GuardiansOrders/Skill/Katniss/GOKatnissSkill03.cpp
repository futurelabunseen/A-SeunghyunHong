// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill03.h"

UGOKatnissSkill03::UGOKatnissSkill03()
{

}

void UGOKatnissSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissSkill03::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOKatnissSkill03::UpdateCast(float DeltaTime)
{
}

void UGOKatnissSkill03::FinishCast()
{
}

void UGOKatnissSkill03::InterruptedCast()
{
}

void UGOKatnissSkill03::ActivateEffect()
{
}

bool UGOKatnissSkill03::IsCasting() const
{
	return false;
}

bool UGOKatnissSkill03::IsCastable() const
{
	return false;
}
