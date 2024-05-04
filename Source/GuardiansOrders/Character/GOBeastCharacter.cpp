// Fill out your copyright notice in the Description page of Project Settings.


#include "GOBeastCharacter.h"
#include "CharacterStat/GOCharacterStatComponent.h"

AGOBeastCharacter::AGOBeastCharacter()
{
	bReplicates = true;
}

void AGOBeastCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetData(FName(TEXT("Beast")));
}

void AGOBeastCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGOBeastCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGOBeastCharacter::OnBaseSkill()
{
	Super::OnBaseSkill();
	UE_LOG(LogTemp, Log, TEXT("Beast Base Skill is triggered."));
}

void AGOBeastCharacter::OnSkillQ()
{
	Super::OnSkillQ();
	UE_LOG(LogTemp, Log, TEXT("Beast Skill Q is triggered."));
}

void AGOBeastCharacter::OnSkillW()
{
	Super::OnSkillW();
	UE_LOG(LogTemp, Log, TEXT("Beast Skill W is triggered."));
}

void AGOBeastCharacter::OnSkillE()
{
	Super::OnSkillE();
	UE_LOG(LogTemp, Log, TEXT("Beast Skill E is triggered."));
}

void AGOBeastCharacter::OnSkillR()
{
	Super::OnSkillR();
	UE_LOG(LogTemp, Log, TEXT("Beast Skill R is triggered."));
}