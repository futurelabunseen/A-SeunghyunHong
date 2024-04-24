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
	//if (GORogersSkill01)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("SkillStat GORogersSkill01 Name: %s"), *GORogersSkill01->Name);
	//	UE_LOG(LogTemp, Warning, TEXT("SkillStat GORogersSkill01 DamageMultiplier: %f"), GORogersSkill01->GetTotalSkillStat().DamageMultiplier);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("GORogersSkill01 is not initialized!"));
	//}
	
}

void AGORogersCharacter::OnSkillQ()
{
	Super::OnSkillQ();
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill Q is triggered."));
}

void AGORogersCharacter::OnSkillW()
{
	Super::OnSkillW();
	// ProcessComboCommand();
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill W is triggered."));
}

void AGORogersCharacter::OnSkillE()
{
	Super::OnSkillE();
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill E is triggered."));
}

void AGORogersCharacter::OnSkillR()
{
	Super::OnSkillR();
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill R is triggered."));
}