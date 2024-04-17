// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissSkill01.h"

UGOKatnissSkill01::UGOKatnissSkill01()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Katniss_Skill01));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Katniss/GOSkill_Katniss_Skill01.GOSkill_Katniss_Skill01"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOKatnissSkill01::PostInitProperties()
{
	Super::PostInitProperties();
}
