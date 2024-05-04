// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersSkill02.h"

UGORogersSkill02::UGORogersSkill02()
{

}

void UGORogersSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersSkill02::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGORogersSkill02::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGORogersSkill02::FinishCast()
{
	Super::FinishCast();
}

void UGORogersSkill02::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGORogersSkill02::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGORogersSkill02::IsCasting() const
//{
//	return false;
//}
//
//bool UGORogersSkill02::IsCastable() const
//{
//	return false;
//}
