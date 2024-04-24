// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideUltimateSkill.h"

UGOBrideUltimateSkill::UGOBrideUltimateSkill()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Bride_UltimateSkill));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Bride/GOSkill_Bride_UltimateSkill.GOSkill_Bride_UltimateSkill"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOBrideUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}
