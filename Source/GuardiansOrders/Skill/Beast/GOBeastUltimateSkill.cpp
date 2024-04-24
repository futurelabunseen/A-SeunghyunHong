// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastUltimateSkill.h"

UGOBeastUltimateSkill::UGOBeastUltimateSkill()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Beast_UltimateSkill));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Beast/GOSkill_Beast_UltimateSkill.GOSkill_Beast_UltimateSkill"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOBeastUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}
