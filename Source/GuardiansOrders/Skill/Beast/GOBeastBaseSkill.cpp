// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastBaseSkill.h"

UGOBeastBaseSkill::UGOBeastBaseSkill()
{

}

void UGOBeastBaseSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBeastBaseSkill::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOBeastBaseSkill::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOBeastBaseSkill::FinishCast()
{
	Super::FinishCast();
}

void UGOBeastBaseSkill::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOBeastBaseSkill::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOBeastBaseSkill::IsCasting() const
//{
//	return false;
//}
//
//bool UGOBeastBaseSkill::IsCastable() const
//{
//	return false;
//}
