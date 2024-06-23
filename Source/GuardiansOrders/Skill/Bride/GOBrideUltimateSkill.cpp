// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideUltimateSkill.h"

UGOBrideUltimateSkill::UGOBrideUltimateSkill()
{

}

void UGOBrideUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBrideUltimateSkill::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOBrideUltimateSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UGOBrideUltimateSkill::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOBrideUltimateSkill::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOBrideUltimateSkill::FinishCast()
{
	Super::FinishCast();
}

void UGOBrideUltimateSkill::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOBrideUltimateSkill::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOBrideUltimateSkill::IsCasting() const
//{
//	return false;
//}
//
//bool UGOBrideUltimateSkill::IsCastable() const
//{
//	return false;
//}
