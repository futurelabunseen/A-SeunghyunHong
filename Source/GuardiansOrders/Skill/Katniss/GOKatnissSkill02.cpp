// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill02.h"

UGOKatnissSkill02::UGOKatnissSkill02()
{
	bIsSpreadSkill = true;
	NumProjectiles = 5;  
	SpreadAngle = 45.0f; 
}

void UGOKatnissSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissSkill02::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOKatnissSkill02::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOKatnissSkill02::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOKatnissSkill02::FinishCast()
{
	Super::FinishCast();
}

void UGOKatnissSkill02::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOKatnissSkill02::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOKatnissSkill02::IsCasting() const
//{
//	return false;
//}
//
//bool UGOKatnissSkill02::IsCastable() const
//{
//	return false;
//}
