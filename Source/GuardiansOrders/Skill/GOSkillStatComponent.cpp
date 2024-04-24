// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillStatComponent.h"
#include "GameData/GOGameSingleton.h"
#include "GameData/GOGameSubsystem.h"

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

	//UGOGameSubsystem* GameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGOGameSubsystem>();
	//CurrentSkillType = FMath::Clamp(InNewSkillType, 1, GameSubsystem->SkillMaxCnt);
	//SetBaseSkillStat(GameSubsystem->GetSkillStat(CurrentSkillType));
}

void UGOSkillStatComponent::ResetSkillStat()
{
	SetSkillStat(CurrentSkillType);
	UE_LOG(LogTemp, Warning, TEXT("SkillStat UGOSkillStatComponent CurrentSkillType: %f"), CurrentSkillType);

}
