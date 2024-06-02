// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersSkill03.h"
#include "Character/GOPlayerCharacter.h"

UGORogersSkill03::UGORogersSkill03()
{
	MovementDistance = 300.0f; // 300 units
	MovementDuration = 2.4f;   // 2 second
	Acceleration = 150.0f;     // 200 units/second^2
}

void UGORogersSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersSkill03::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
}

void UGORogersSkill03::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);


}

void UGORogersSkill03::ActivateSkill()
{
	Super::ActivateSkill();

	if (AGOPlayerCharacter* Owner = Cast<AGOPlayerCharacter>(GetSkillOwner()))
	{
		Owner->StartMovingForward(MovementDistance, MovementDuration, Acceleration);
	}
}

void UGORogersSkill03::FinishCast()
{
	Super::FinishCast();
}

void UGORogersSkill03::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGORogersSkill03::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGORogersSkill03::IsCasting() const
//{
//	return false;
//}
//
//bool UGORogersSkill03::IsCastable() const
//{
//	return false;
//}
