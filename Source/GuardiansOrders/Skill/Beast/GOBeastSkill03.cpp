// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastSkill03.h"
#include "Character/GOPlayerCharacter.h"

UGOBeastSkill03::UGOBeastSkill03()
{
	MovementDistance = 300.0f; // 200 units
	MovementDuration = 1.4f;   // 1.5 seconds
	Acceleration = 200.0f;     // 150 units/second^2
}

void UGOBeastSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOBeastSkill03::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGOBeastSkill03::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOBeastSkill03::ActivateSkill()
{
	Super::ActivateSkill();

	if (AGOPlayerCharacter* Owner = Cast<AGOPlayerCharacter>(GetSkillOwner()))
	{
		Owner->StartMovingForward(MovementDistance, MovementDuration, Acceleration);
	}
}

void UGOBeastSkill03::FinishCast()
{
	Super::FinishCast();
}

void UGOBeastSkill03::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOBeastSkill03::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOBeastSkill03::IsCasting() const
//{
//	return false;
//}
//
//bool UGOBeastSkill03::IsCastable() const
//{
//	return false;
//}
