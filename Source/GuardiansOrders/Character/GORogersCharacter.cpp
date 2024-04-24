// Fill out your copyright notice in the Description page of Project Settings.


#include "GORogersCharacter.h"
#include "CharacterStat/GOCharacterStatComponent.h"

AGORogersCharacter::AGORogersCharacter()
{
	bReplicates = true;
}

void AGORogersCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetData(FName(TEXT("Rogers")));
}

void AGORogersCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGORogersCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SkillQInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillStat GORogersSkillQ Name: %s"), *SkillQInstance->GetTotalSkillData().SkillName);
		UE_LOG(LogTemp, Warning, TEXT("SkillStat GORogersSkillQ DamageMultiplier: %f"), SkillQInstance->GetTotalSkillStat().DamageMultiplier);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GORogersSkill01 is not initialized!"));
	}
}

void AGORogersCharacter::OnSkillQ()
{
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill Q is triggered."));
}

void AGORogersCharacter::OnSkillW()
{
	// ProcessComboCommand();
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill W is triggered."));
}

void AGORogersCharacter::OnSkillE()
{
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill E is triggered."));
}

void AGORogersCharacter::OnSkillR()
{
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill R is triggered."));
}