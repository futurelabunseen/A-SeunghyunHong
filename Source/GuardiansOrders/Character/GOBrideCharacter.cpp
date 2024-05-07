// Fill out your copyright notice in the Description page of Project Settings.


#include "GOBrideCharacter.h"
#include "CharacterStat/GOCharacterStatComponent.h"
#include "GOCharacterMovementComponent.h"

AGOBrideCharacter::AGOBrideCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGOCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	bReplicates = true;
}

void AGOBrideCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetData(FName(TEXT("Bride")));
}

void AGOBrideCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGOBrideCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGOBrideCharacter::OnBaseSkill()
{
	Super::OnBaseSkill();
	UE_LOG(LogTemp, Log, TEXT("Bride Base Skill is triggered."));
}

void AGOBrideCharacter::OnSkillQ()
{
	Super::OnSkillQ();
	UE_LOG(LogTemp, Log, TEXT("Bride Skill Q is triggered."));
}

void AGOBrideCharacter::OnSkillW()
{
	Super::OnSkillW();
	UE_LOG(LogTemp, Log, TEXT("Bride Skill W is triggered."));
}

void AGOBrideCharacter::OnSkillE()
{
	Super::OnSkillE();
	UE_LOG(LogTemp, Log, TEXT("Bride Skill E is triggered."));
}

void AGOBrideCharacter::OnSkillR()
{
	Super::OnSkillR();
	UE_LOG(LogTemp, Log, TEXT("Bride Skill R is triggered."));
}



