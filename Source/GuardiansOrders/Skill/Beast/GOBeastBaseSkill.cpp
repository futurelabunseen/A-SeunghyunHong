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
}

void UGOBeastBaseSkill::UpdateCast(float DeltaTime)
{
}

void UGOBeastBaseSkill::FinishCast()
{
}

void UGOBeastBaseSkill::InterruptedCast()
{
}

void UGOBeastBaseSkill::ActivateEffect()
{
}

bool UGOBeastBaseSkill::IsCasting() const
{
	return false;
}

bool UGOBeastBaseSkill::IsCastable() const
{
	return false;
}
