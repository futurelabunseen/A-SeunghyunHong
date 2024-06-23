// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersUltimateSkill.h"

UGORogersUltimateSkill::UGORogersUltimateSkill()
{

}

void UGORogersUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersUltimateSkill::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGORogersUltimateSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UGORogersUltimateSkill::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGORogersUltimateSkill::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGORogersUltimateSkill::FinishCast()
{
	Super::FinishCast();
}

void UGORogersUltimateSkill::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGORogersUltimateSkill::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGORogersUltimateSkill::IsCasting() const
//{
//	return false;
//}
//
//bool UGORogersUltimateSkill::IsCastable() const
//{
//	return false;
//}
