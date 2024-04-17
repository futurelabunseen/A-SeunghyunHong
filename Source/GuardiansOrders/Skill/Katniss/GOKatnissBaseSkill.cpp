// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Katniss/GOKatnissBaseSkill.h"

UGOKatnissBaseSkill::UGOKatnissBaseSkill()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Katniss_BaseSkill));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Katniss/GOSkill_Katniss_BaseSkill.GOSkill_Katniss_BaseSkill"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOKatnissBaseSkill::PostInitProperties()
{
	Super::PostInitProperties();
}
