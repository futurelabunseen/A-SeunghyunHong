// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideSkill03.h"

UGOBrideSkill03::UGOBrideSkill03()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Bride_Skill03));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Bride/GOSkill_Bride_Skill03.GOSkill_Bride_Skill03"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOBrideSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}
