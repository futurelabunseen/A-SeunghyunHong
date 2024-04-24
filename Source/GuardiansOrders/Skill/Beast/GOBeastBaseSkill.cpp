// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Beast/GOBeastBaseSkill.h"

UGOBeastBaseSkill::UGOBeastBaseSkill()
{
	SetCurrentSkillType(static_cast<int32>(EGOHeroSkillType::GOSkill_Beast_BaseSkill));

	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Beast/GOSkill_Beast_BaseSkill.GOSkill_Beast_BaseSkill"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
	}
}

void UGOBeastBaseSkill::PostInitProperties()
{
	Super::PostInitProperties();
}
