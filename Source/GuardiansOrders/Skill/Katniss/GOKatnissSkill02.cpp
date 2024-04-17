// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill02.h"

UGOKatnissSkill02::UGOKatnissSkill02()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Katniss_Skill02));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Katniss/GOSkill_Katniss_Skill02.GOSkill_Katniss_Skill02"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOKatnissSkill02::PostInitProperties()
{
	Super::PostInitProperties();
}
