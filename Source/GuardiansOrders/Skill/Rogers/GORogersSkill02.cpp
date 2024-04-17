// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersSkill02.h"

UGORogersSkill02::UGORogersSkill02()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Rogers_Skill02));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Rogers/GOSkill_Rogers_Skill02.GOSkill_Rogers_Skill02"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGORogersSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}
