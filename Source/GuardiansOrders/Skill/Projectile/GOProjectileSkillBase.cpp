// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Projectile/GOProjectileSkillBase.h"
#include "Projectile/GOProjectile.h"

UGOProjectileSkillBase::UGOProjectileSkillBase()
{
    SpreadAngle = 45.0f;  // Default spread angle
    NumProjectiles = 1;   // Default number of projectiles
    bIsSpreadSkill = false; // Default to not spread
}

void UGOProjectileSkillBase::PostInitProperties()
{
    Super::PostInitProperties();
    SetSpeed();
}

void UGOProjectileSkillBase::StartCast()
{
    Super::StartCast();
}

void UGOProjectileSkillBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void UGOProjectileSkillBase::UpdateCast(float DeltaTime)
{
    Super::UpdateCast(DeltaTime);
}

void UGOProjectileSkillBase::ActivateSkill()
{
    Super::ActivateSkill();
}

void UGOProjectileSkillBase::FinishCast()
{
    Super::FinishCast();
}

void UGOProjectileSkillBase::InterruptedCast()
{
    Super::InterruptedCast();
}

void UGOProjectileSkillBase::ActivateEffect()
{
    Super::ActivateEffect();
}

void UGOProjectileSkillBase::SetSpeed()
{

}