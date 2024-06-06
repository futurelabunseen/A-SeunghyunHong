// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissUltimateSkill.h"

UGOKatnissUltimateSkill::UGOKatnissUltimateSkill()
{
	SpreadAngle = 0.0f; 
	NumProjectiles = 7;  
	bIsSpreadSkill = false; 
}

void UGOKatnissUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissUltimateSkill::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOKatnissUltimateSkill::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOKatnissUltimateSkill::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOKatnissUltimateSkill::FinishCast()
{
	Super::FinishCast();
}

void UGOKatnissUltimateSkill::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOKatnissUltimateSkill::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOKatnissUltimateSkill::IsCasting() const
//{
//	return false;
//}
//
//bool UGOKatnissUltimateSkill::IsCastable() const
//{
//	return false;
//}
