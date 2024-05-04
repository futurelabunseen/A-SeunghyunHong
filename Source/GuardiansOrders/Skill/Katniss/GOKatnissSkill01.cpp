// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill01.h"

UGOKatnissSkill01::UGOKatnissSkill01()
{

}

void UGOKatnissSkill01::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissSkill01::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOKatnissSkill01::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOKatnissSkill01::FinishCast()
{
	Super::FinishCast();
}

void UGOKatnissSkill01::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOKatnissSkill01::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOKatnissSkill01::IsCasting() const
//{
//	return false;
//}
//
//bool UGOKatnissSkill01::IsCastable() const
//{
//	return false;
//}
