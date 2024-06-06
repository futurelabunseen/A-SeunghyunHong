// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill03.h"
#include <Skill/GOSkillCastComponent.h>

UGOKatnissSkill03::UGOKatnissSkill03()
{
	bIsSpreadSkill = false;
	NumProjectiles = 7;
}

void UGOKatnissSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissSkill03::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();
	ProjectilesFired = 0;
	SpawnSequentialProjectiles();
}

void UGOKatnissSkill03::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOKatnissSkill03::ActivateSkill()
{
	Super::ActivateSkill();

}

void UGOKatnissSkill03::FinishCast()
{
	Super::FinishCast();
	GetWorld()->GetTimerManager().ClearTimer(ProjectileTimerHandle);
}

void UGOKatnissSkill03::InterruptedCast()
{
	Super::InterruptedCast();
	GetWorld()->GetTimerManager().ClearTimer(ProjectileTimerHandle);
}

void UGOKatnissSkill03::ActivateEffect()
{
	Super::ActivateEffect();
}

void UGOKatnissSkill03::SpawnSequentialProjectiles()
{
	if (ProjectilesFired < NumProjectiles)
	{
		if (GetSkillOwner())
		{
			FVector Location = GetSkillOwner()->GetActorLocation();
			FRotator Rotation = GetSkillOwner()->GetActorRotation();

			// Introduce a slight random variation in the rotation
			float RandomYawOffset = FMath::FRandRange(-10.0f, 10.0f); // Adjust the range as needed for more or less variation
			Rotation.Yaw += RandomYawOffset;

			SpawnProjectile(Location, Rotation);

			ProjectilesFired++;
			GetWorld()->GetTimerManager().SetTimer(ProjectileTimerHandle, this, &UGOKatnissSkill03::SpawnSequentialProjectiles, 0.1f, false);
		}
	}
}

void UGOKatnissSkill03::SpawnProjectile(FVector Location, FRotator Rotation)
{
	if (UGOSkillCastComponent* SkillCastComponent = GetSkillOwner()->FindComponentByClass<UGOSkillCastComponent>())
	{
		SkillCastComponent->HandleProjectileSkill(this, Location, Rotation, FTransform(Rotation, Location));
	}
}
//bool UGOKatnissSkill03::IsCasting() const
//{
//	return false;
//}
//
//bool UGOKatnissSkill03::IsCastable() const
//{
//	return false;
//}
