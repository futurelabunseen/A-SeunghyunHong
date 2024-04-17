// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersBaseSkill.h"

UGORogersBaseSkill::UGORogersBaseSkill()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Rogers_Skill01));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Rogers/GOSkill_Rogers_Skill01.GOSkill_Rogers_Skill01"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGORogersBaseSkill::PostInitProperties()
{
	Super::PostInitProperties();
}
