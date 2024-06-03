// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Projectile/GOProjectileSkillBase.h"
#include "Projectile/GOProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

UGOProjectileSkillBase::UGOProjectileSkillBase()
{
}

void UGOProjectileSkillBase::PostInitProperties()
{
    Super::PostInitProperties();
}

void UGOProjectileSkillBase::StartCast()
{
    Super::StartCast();
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