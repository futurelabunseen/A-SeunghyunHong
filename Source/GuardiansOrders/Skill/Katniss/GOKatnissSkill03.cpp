// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill03.h"

UGOKatnissSkill03::UGOKatnissSkill03()
{

}

void UGOKatnissSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissSkill03::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOKatnissSkill03::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOKatnissSkill03::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOKatnissSkill03::FinishCast()
{
	Super::FinishCast();
}

void UGOKatnissSkill03::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOKatnissSkill03::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOKatnissSkill03::IsCasting() const
//{
//	return false;
//}
//
//bool UGOKatnissSkill03::IsCastable() const
//{
//	return false;
//}
