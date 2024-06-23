// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill01.h"
#include <Skill/GOSkillCastComponent.h>

UGOKatnissSkill01::UGOKatnissSkill01()
{
	bIsSpreadSkill = false;
	bIsAreaSkill = true;
	NumProjectiles = 0;
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
	StartArrowRain();

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

void UGOKatnissSkill01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UGOKatnissSkill01::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOKatnissSkill01::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOKatnissSkill01::FinishCast()
{
	Super::FinishCast();
	GetWorld()->GetTimerManager().ClearTimer(ArrowRainTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
}

void UGOKatnissSkill01::InterruptedCast()
{
	Super::InterruptedCast();
	GetWorld()->GetTimerManager().ClearTimer(ArrowRainTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
}

void UGOKatnissSkill01::ActivateEffect()
{
	Super::ActivateEffect();
}

void UGOKatnissSkill01::StartArrowRain()
{
	if (GetSkillOwner())
	{
		SkillLocation = GetSkillOwner()->GetActorLocation(); // Get the location to spawn the rain


		if (UGOSkillCastComponent* SkillCastComponent = GetSkillOwner()->FindComponentByClass<UGOSkillCastComponent>())
		{
			//SkillCastComponent->StartArrowRain(SkillLocation, Duration, SpawnInterval, NumProjectilesPerSpawn);
		}
	}
}

void UGOKatnissSkill01::EndArrowRain()
{
	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
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
