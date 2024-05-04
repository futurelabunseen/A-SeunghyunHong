// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersSkill01.h"
#include "Share/EGOSkill.h"

UGORogersSkill01::UGORogersSkill01()
{

}

void UGORogersSkill01::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersSkill01::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGORogersSkill01::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGORogersSkill01::FinishCast()
{
	Super::FinishCast();
}

void UGORogersSkill01::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGORogersSkill01::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGORogersSkill01::IsCasting() const
//{
//	return false;
//}
//
//bool UGORogersSkill01::IsCastable() const
//{
//	return false;
//}
