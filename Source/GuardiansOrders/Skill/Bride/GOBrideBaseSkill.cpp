// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideBaseSkill.h"

UGOBrideBaseSkill::UGOBrideBaseSkill()
{

}

void UGOBrideBaseSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBrideBaseSkill::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOBrideBaseSkill::UpdateCast(float DeltaTime)
{
}

void UGOBrideBaseSkill::FinishCast()
{
}

void UGOBrideBaseSkill::InterruptedCast()
{
}

void UGOBrideBaseSkill::ActivateEffect()
{
}

bool UGOBrideBaseSkill::IsCasting() const
{
	return false;
}

bool UGOBrideBaseSkill::IsCastable() const
{
	return false;
}
