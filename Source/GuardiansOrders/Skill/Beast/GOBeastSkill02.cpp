// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastSkill02.h"

UGOBeastSkill02::UGOBeastSkill02()
{

}

void UGOBeastSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBeastSkill02::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOBeastSkill02::UpdateCast(float DeltaTime)
{
}

void UGOBeastSkill02::FinishCast()
{
}

void UGOBeastSkill02::InterruptedCast()
{
}

void UGOBeastSkill02::ActivateEffect()
{
}

bool UGOBeastSkill02::IsCasting() const
{
	return false;
}

bool UGOBeastSkill02::IsCastable() const
{
	return false;
}
