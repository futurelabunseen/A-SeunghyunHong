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
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOKatnissBaseSkill::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOKatnissBaseSkill::FinishCast()
{
	Super::FinishCast();
}

void UGOKatnissBaseSkill::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOKatnissBaseSkill::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOKatnissBaseSkill::IsCasting() const
//{
//	return false;
//}
//
//bool UGOKatnissBaseSkill::IsCastable() const
//{
//	return false;
//}
