// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Rogers/GORogersSkill03.h"

UGORogersSkill03::UGORogersSkill03()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Rogers_Skill03));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Rogers/GOSkill_Rogers_Skill03.GOSkill_Rogers_Skill03"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGORogersSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}
