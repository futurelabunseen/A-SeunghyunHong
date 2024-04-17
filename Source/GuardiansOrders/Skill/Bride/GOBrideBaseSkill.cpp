// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Bride/GOBrideBaseSkill.h"

UGOBrideBaseSkill::UGOBrideBaseSkill()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Bride_BaseSkill));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Bride/GOSkill_Bride_BaseSkill.GOSkill_Bride_BaseSkill"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOBrideBaseSkill::PostInitProperties()
{
	Super::PostInitProperties();
}
