// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GORogersSkill01.h"
#include "Skill/GOSkillStatComponent.h"
#include "Share/EGOSkill.h"

UGORogersSkill01::UGORogersSkill01()
{
	static ConstructorHelpers::FObjectFinder<UGOSkillDataAsset> DataAsset(TEXT("/Game/GameData/SkillDataAsset/Rogers/GOSkill_Rogers_Skill01.GOSkill_Rogers_Skill01"));
	if (DataAsset.Succeeded())
	{
		SkillDataAsset = DataAsset.Object;
        if (SkillStat)
        {
            SkillStat->SetCurrentSkillType(static_cast<int8>(EGOHeroSkillType::GOSkill_Rogers_Skill01));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SkillStat component is not initialized!"));
        }
    }
}