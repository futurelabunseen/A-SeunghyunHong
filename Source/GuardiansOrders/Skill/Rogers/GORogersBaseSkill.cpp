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
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGORogersBaseSkill::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGORogersBaseSkill::ActivateSkill()
{
	Super::ActivateSkill();
}

void UGORogersBaseSkill::FinishCast()
{
	Super::FinishCast();
}

void UGORogersBaseSkill::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGORogersBaseSkill::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGORogersBaseSkill::IsCasting() const
//{
//	return false;
//}
//
//bool UGORogersBaseSkill::IsCastable() const
//{
//	return false;
//}
