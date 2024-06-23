// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastUltimateSkill.h"

UGOBeastUltimateSkill::UGOBeastUltimateSkill()
{

}

void UGOBeastUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBeastUltimateSkill::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOBeastUltimateSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UGOBeastUltimateSkill::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOBeastUltimateSkill::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOBeastUltimateSkill::FinishCast()
{
	Super::FinishCast();
}

void UGOBeastUltimateSkill::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOBeastUltimateSkill::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOBeastUltimateSkill::IsCasting() const
//{
//	return false;
//}
//
//bool UGOBeastUltimateSkill::IsCastable() const
//{
//	return false;
//}
