// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideSkill02.h"

UGOBrideSkill02::UGOBrideSkill02()
{

}

void UGOBrideSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBrideSkill02::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOBrideSkill02::UpdateCast(float DeltaTime)
{
}

void UGOBrideSkill02::FinishCast()
{
}

void UGOBrideSkill02::InterruptedCast()
{
}

void UGOBrideSkill02::ActivateEffect()
{
}

bool UGOBrideSkill02::IsCasting() const
{
	return false;
}

bool UGOBrideSkill02::IsCastable() const
{
	return false;
}
