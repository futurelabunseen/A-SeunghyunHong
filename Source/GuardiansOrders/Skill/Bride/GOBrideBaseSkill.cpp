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
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOBrideBaseSkill::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOBrideBaseSkill::FinishCast()
{
	Super::FinishCast();
}

void UGOBrideBaseSkill::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOBrideBaseSkill::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOBrideBaseSkill::IsCasting() const
//{
//	return false;
//}
//
//bool UGOBrideBaseSkill::IsCastable() const
//{
//	return false;
//}
