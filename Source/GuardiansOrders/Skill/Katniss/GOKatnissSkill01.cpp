// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill01.h"

UGOKatnissSkill01::UGOKatnissSkill01()
{

}

void UGOKatnissSkill01::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissSkill01::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	
	
	Super::StartCast();

	//if (IsCastable() == false)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Skill is not castable due to cooldown or other conditions."));
	//	return;
	//}
	//UE_LOG(LogTemp, Log, TEXT("[SkillSystem] UGOSkillBase::StartCast() is Called."));

	//bIsCasting = true;
	//SetCoolDownTimer();
	//OnCooldownUpdated.Broadcast(CoolDownTimer);  // 쿨다운 시작 시 즉시 UI 업데이트

	//if (GetWorld()->GetTimerManager().IsTimerActive(CoolDownTickTimerHandle))
	//{
	//	GetWorld()->GetTimerManager().ClearTimer(CoolDownTickTimerHandle);
	//}
	//GetWorld()->GetTimerManager().SetTimer(CoolDownTickTimerHandle, this, &UGOSkillBase::CheckCooldownTick, 0.1f, true);
}

void UGOKatnissSkill01::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOKatnissSkill01::FinishCast()
{
	Super::FinishCast();
}

void UGOKatnissSkill01::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOKatnissSkill01::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOKatnissSkill01::IsCasting() const
//{
//	return false;
//}
//
//bool UGOKatnissSkill01::IsCastable() const
//{
//	return false;
//}
