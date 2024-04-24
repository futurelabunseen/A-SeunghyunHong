// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill03.h"

UGOKatnissSkill03::UGOKatnissSkill03()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Katniss_Skill03));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Katniss/GOSkill_Katniss_Skill03.GOSkill_Katniss_Skill03"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOKatnissSkill03::PostInitProperties()
{
	Super::PostInitProperties();
}
