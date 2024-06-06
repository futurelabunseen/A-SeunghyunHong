// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill02.h"
#include <Skill/GOSkillCastComponent.h>

UGOKatnissSkill02::UGOKatnissSkill02()
{
	bIsSpreadSkill = true;
	NumProjectiles = 5;  
	SpreadAngle = 45.0f; 
}

void UGOKatnissSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOKatnissSkill02::StartCast()
{
	//SetCoolDownTimer();
	//bIsCasting = true;
	Super::StartCast();

}

void UGOKatnissSkill02::UpdateCast(float DeltaTime)
{
	Super::UpdateCast(DeltaTime);
}

void UGOKatnissSkill02::ActivateSkill()
{
	Super::ActivateSkill();

	if (GetSkillOwner())
	{
		FVector Location = GetSkillOwner()->GetActorLocation();
		FRotator Rotation = GetSkillOwner()->GetActorRotation();
		//SpawnSpreadProjectiles(Location, Rotation);
	}
	UE_LOG(LogTemp, Warning, TEXT("[Projectile] UGOKatnissSkill02::StartCast"));
}

void UGOKatnissSkill02::FinishCast()
{
	Super::FinishCast();
}

void UGOKatnissSkill02::InterruptedCast()
{
	Super::InterruptedCast();
}

void UGOKatnissSkill02::ActivateEffect()
{
	Super::ActivateEffect();
}

//bool UGOKatnissSkill02::IsCasting() const
//{
//	return false;
//}
//
//bool UGOKatnissSkill02::IsCastable() const
//{
//	return false;
//}
void UGOKatnissSkill02::SpawnSpreadProjectiles(FVector Location, FRotator Rotation)
{
	UE_LOG(LogTemp, Warning, TEXT("[Projectile] UGOKatnissSkill02::SpawnSpreadProjectiles"));
	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpreadAngle / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpreadAngle / 2.f, FVector::UpVector);

	if (NumProjectiles > 1)
	{
		const float DeltaSpread = SpreadAngle / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			FVector SpawnLocation = Location + FVector(0, 0, 50); // Adjust the spawn location as needed
			FRotator SpawnRotation = Direction.Rotation();
			FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

			// Call the component to handle the projectile spawn
			if (UGOSkillCastComponent* SkillCastComponent = GetSkillOwner()->FindComponentByClass<UGOSkillCastComponent>())
			{
				SkillCastComponent->HandleProjectileSkill(this, SpawnLocation, SpawnRotation, SpawnTransform);
			}
		}
	}
	else
	{
		// Single projectile
		FTransform SpawnTransform = FTransform(Rotation, Location);
		if (UGOSkillCastComponent* SkillCastComponent = GetSkillOwner()->FindComponentByClass<UGOSkillCastComponent>())
		{
			SkillCastComponent->HandleProjectileSkill(this, Location, Rotation, SpawnTransform);
		}
	}
}