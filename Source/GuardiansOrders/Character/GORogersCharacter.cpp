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

void AGORogersCharacter::PlaySkillAnim(UGOSkillBase* CurrentSkill)
{
	GetMesh()->GetAnimInstance()->Montage_Play(CurrentSkill->GetTotalSkillData().SkillAnim);
	UE_LOG(LogTemp, Warning, TEXT("[AGORogersCharacter::PlaySkillAnim]  1 called. This function is inherited from GOPlaySkillAnimInterface. "));
}
