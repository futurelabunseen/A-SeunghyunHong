// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersUltimateSkill.h"

UGORogersUltimateSkill::UGORogersUltimateSkill()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Rogers_UltimateSkill));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Rogers/GOSkill_Rogers_UltimateSkill.GOSkill_Rogers_UltimateSkill"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGORogersUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}
