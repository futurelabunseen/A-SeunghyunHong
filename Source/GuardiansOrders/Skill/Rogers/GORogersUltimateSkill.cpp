// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersUltimateSkill.h"

UGORogersUltimateSkill::UGORogersUltimateSkill()
{

}

void UGORogersUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersUltimateSkill::StartCast()
{
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] Start Skill Casting: UGORogersSkill02"));
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] StartCast: Montage Name: %s"), *GetTotalSkillData().SkillAnim.GetName());
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] StartCast: SkillName Name: %s"), *GetTotalSkillData().SkillName);

	SetCoolDownTimer();
	bIsCasting = true;
}

void UGORogersUltimateSkill::UpdateCast(float DeltaTime)
{
	if (!bIsCasting) return;

}

void UGORogersUltimateSkill::FinishCast()
{
}

void UGORogersUltimateSkill::InterruptedCast()
{
}

void UGORogersUltimateSkill::ActivateEffect()
{
}

bool UGORogersUltimateSkill::IsCasting() const
{
	return bIsCasting;
}

bool UGORogersUltimateSkill::IsCastable() const
{
	return GetCoolDownTimer() <= 0;
}
