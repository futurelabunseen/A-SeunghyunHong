// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastSkill02.h"

UGOBeastSkill02::UGOBeastSkill02()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Beast_Skill02));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Beast/GOSkill_Beast_Skill02.GOSkill_Beast_Skill02"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOBeastSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}
