// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill02.h"

UGOKatnissSkill02::UGOKatnissSkill02()
{

}

void UGOKatnissSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissSkill02::StartCast()
{
	SetCoolDownTimer();
	bIsCasting = true;
}

void UGOKatnissSkill02::UpdateCast(float DeltaTime)
{
}

void UGOKatnissSkill02::FinishCast()
{
}

void UGOKatnissSkill02::InterruptedCast()
{
}

void UGOKatnissSkill02::ActivateEffect()
{
}

bool UGOKatnissSkill02::IsCasting() const
{
	return false;
}

bool UGOKatnissSkill02::IsCastable() const
{
	return false;
}
