// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideSkill01.h"

UGOBrideSkill01::UGOBrideSkill01()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Bride_Skill01));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Bride/GOSkill_Bride_Skill01.GOSkill_Bride_Skill01"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOBrideSkill01::PostInitProperties()
{
	Super::PostInitProperties();
}
