// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastSkill01.h"

UGOBeastSkill01::UGOBeastSkill01()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Beast_Skill01));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Beast/GOSkill_Beast_Skill01.GOSkill_Beast_Skill01"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOBeastSkill01::PostInitProperties()
{
	Super::PostInitProperties();
}
