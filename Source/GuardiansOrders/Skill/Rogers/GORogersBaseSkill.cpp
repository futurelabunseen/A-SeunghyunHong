// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersBaseSkill.h"

UGORogersBaseSkill::UGORogersBaseSkill()
{

}

void UGORogersBaseSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersBaseSkill::StartCast()
{
}

void UGORogersBaseSkill::UpdateCast(float DeltaTime)
{
}

void UGORogersBaseSkill::FinishCast()
{
}

void UGORogersBaseSkill::InterruptedCast()
{
}

void UGORogersBaseSkill::ActivateEffect()
{
}

bool UGORogersBaseSkill::IsCasting() const
{
	return bIsCasting;
}

bool UGORogersBaseSkill::IsCastable() const
{
	return GetCoolDownTimer() <= 0;
}
