// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideSkill02.h"

UGOBrideSkill02::UGOBrideSkill02()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Bride_Skill02));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Bride/GOSkill_Bride_Skill02.GOSkill_Bride_Skill02"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOBrideSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}
