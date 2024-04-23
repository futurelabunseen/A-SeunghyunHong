// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersSkill03.h"

UGORogersSkill03::UGORogersSkill03()
{

}

void UGORogersSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersSkill03::StartCast()
{
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] Start Skill Casting: UGORogersSkill03"));

	SetCoolDownTimer();
	bIsCasting = true;
}

void UGORogersSkill03::UpdateCast(float DeltaTime)
{
	if (!bIsCasting) return;
}

void UGORogersSkill03::FinishCast()
{
	bIsCasting = false;
}

void UGORogersSkill03::InterruptedCast()
{
	bIsCasting = false;
}

void UGORogersSkill03::ActivateEffect()
{
}

bool UGORogersSkill03::IsCasting() const
{
	return bIsCasting;
}

bool UGORogersSkill03::IsCastable() const
{
	return GetCoolDownTimer() <= 0;
}
