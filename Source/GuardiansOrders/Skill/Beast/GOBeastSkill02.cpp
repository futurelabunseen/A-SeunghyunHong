// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastSkill02.h"

UGOBeastSkill02::UGOBeastSkill02()
{

}

void UGOBeastSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBeastSkill02::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOBeastSkill02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UGOBeastSkill02::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOBeastSkill02::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOBeastSkill02::FinishCast()
{
	Super::FinishCast();
}

void UGOBeastSkill02::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOBeastSkill02::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOBeastSkill02::IsCasting() const
//{
//	return false;
//}
//
//bool UGOBeastSkill02::IsCastable() const
//{
//	return false;
//}
