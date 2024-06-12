// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersSkill02.h"
#include <Character/GORogersCharacter.h>

UGORogersSkill02::UGORogersSkill02()
{

}

void UGORogersSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGORogersSkill02::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();

	// ��ų �����ڰ� AGORogersCharacter Ÿ������ Ȯ��
	if (AGORogersCharacter* RogersCharacter = Cast<AGORogersCharacter>(GetSkillOwner()))
	{
		RogersCharacter->ActivateShield();
		FTimerHandle TimerHandle;
		RogersCharacter->GetWorldTimerManager().SetTimer(TimerHandle, this, &UGORogersSkill02::ShieldSkillFinished, 5.0f, false);
	}
}

void UGORogersSkill02::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGORogersSkill02::ActivateSkill()
{
	Super::ActivateSkill();
}


void UGORogersSkill02::FinishCast()
{
	Super::FinishCast();
	UE_LOG(LogTemp, Log, TEXT("[Shield] UGORogersSkill02::FinishCast()"));

	if (AGORogersCharacter* RogersCharacter = Cast<AGORogersCharacter>(GetSkillOwner()))
	{
		RogersCharacter->DeactivateShield();
	}
}

void UGORogersSkill02::InterruptedCast()
{
	Super::InterruptedCast();

	if (AGORogersCharacter* RogersCharacter = Cast<AGORogersCharacter>(GetSkillOwner()))
	{
		RogersCharacter->DeactivateShield();
	}
}

void UGORogersSkill02::ActivateEffect()
{
	Super::ActivateEffect();
}

void UGORogersSkill02::ShieldSkillFinished()
{
	FinishCast();
}

//bool UGORogersSkill02::IsCasting() const
//{
//	return false;
//}
//
//bool UGORogersSkill02::IsCastable() const
//{
//	return false;
//}
