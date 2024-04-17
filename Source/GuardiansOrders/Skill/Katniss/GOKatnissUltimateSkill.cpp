// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissUltimateSkill.h"

UGOKatnissUltimateSkill::UGOKatnissUltimateSkill()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Katniss_UltimateSkill));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Katniss/GOSkill_Katniss_UltimateSkill.GOSkill_Katniss_UltimateSkill"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOKatnissUltimateSkill::PostInitProperties()
{
	Super::PostInitProperties();
}
