// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillStatComponent.h"
#include "GameData/GOGameSingleton.h"

UGOSkillStatComponent::UGOSkillStatComponent()
{
	bWantsInitializeComponent = true;
}

void UGOSkillStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGOSkillStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	ResetSkillStat();
}

void UGOSkillStatComponent::SetSkillStat(int8 InNewSkillType)
{
	CurrentSkillType = FMath::Clamp(InNewSkillType, 1, UGOGameSingleton::Get().CharacterMaxCnt);
	SetBaseSkillStat(UGOGameSingleton::Get().GetSkillStat(CurrentSkillType));
}

void UGOSkillStatComponent::ResetSkillStat()
{
	SetSkillStat(CurrentSkillType);
}
