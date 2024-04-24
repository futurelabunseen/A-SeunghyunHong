// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersSkill01.h"
#include "Skill/GOSkillStatComponent.h"
#include "Share/EGOSkill.h"

UGORogersSkill01::UGORogersSkill01()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Rogers_Skill01));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Rogers/GOSkill_Rogers_Skill01.GOSkill_Rogers_Skill01"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
    }
}

void UGORogersSkill01::PostInitProperties()
{
	Super::PostInitProperties();
}
