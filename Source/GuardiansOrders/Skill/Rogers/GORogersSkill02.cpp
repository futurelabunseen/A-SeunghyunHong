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
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] Start Skill Casting: UGORogersSkill02"));

	SetCoolDownTimer();
	bIsCasting = true;
}

void UGORogersSkill02::UpdateCast(float DeltaTime)
{
	if (!bIsCasting) return;


}

void UGORogersSkill02::FinishCast()
{
	bIsCasting = false;
}

void UGORogersSkill02::InterruptedCast()
{
	bIsCasting = false;
}

void UGORogersSkill02::ActivateEffect()
{
}

bool UGORogersSkill02::IsCasting() const
{
	return bIsCasting;
}

bool UGORogersSkill02::IsCastable() const
{
	return GetCoolDownTimer() <= 0;
}