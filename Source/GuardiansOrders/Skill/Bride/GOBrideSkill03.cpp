// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideSkill03.h"

UGOBrideSkill03::UGOBrideSkill03()
{

}

void UGOBrideSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBrideSkill03::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOBrideSkill03::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UGOBrideSkill03::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOBrideSkill03::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOBrideSkill03::FinishCast()
{
	Super::FinishCast();
}

void UGOBrideSkill03::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOBrideSkill03::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOBrideSkill03::IsCasting() const
//{
//	return false;
//}
//
//bool UGOBrideSkill03::IsCastable() const
//{
//	return false;
//}
