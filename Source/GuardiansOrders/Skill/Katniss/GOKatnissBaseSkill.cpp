// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissBaseSkill.h"

UGOKatnissBaseSkill::UGOKatnissBaseSkill()
{

}

void UGOKatnissBaseSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissBaseSkill::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOKatnissBaseSkill::UpdateCast(float DeltaTime)
{
}

void UGOKatnissBaseSkill::FinishCast()
{
}

void UGOKatnissBaseSkill::InterruptedCast()
{
}

void UGOKatnissBaseSkill::ActivateEffect()
{
}

bool UGOKatnissBaseSkill::IsCasting() const
{
	return false;
}

bool UGOKatnissBaseSkill::IsCastable() const
{
	return false;
}
