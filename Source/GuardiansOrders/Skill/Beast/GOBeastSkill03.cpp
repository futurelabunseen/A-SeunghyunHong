// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastSkill03.h"

UGOBeastSkill03::UGOBeastSkill03()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Beast_Skill03));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Beast/GOSkill_Beast_Skill03.GOSkill_Beast_Skill03"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOBeastSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}
