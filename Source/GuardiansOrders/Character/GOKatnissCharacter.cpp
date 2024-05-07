// Fill out your copyright notice in the Description page of Project Settings.


#include "GOKatnissCharacter.h"
#include "CharacterStat/GOCharacterStatComponent.h"
#include "GOCharacterMovementComponent.h"

AGOKatnissCharacter::AGOKatnissCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGOCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	bReplicates = true;
}

void AGOKatnissCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetData(FName(TEXT("Katniss")));
}

void AGOKatnissCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGOKatnissCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("AGOKatnissCharacter BeginPlay"));

}

void AGOKatnissCharacter::OnBaseSkill()
{
	Super::OnBaseSkill();
	UE_LOG(LogTemp, Log, TEXT("Katniss Base Skill is triggered."));
}

void AGOKatnissCharacter::OnSkillQ()
{
	Super::OnSkillQ();
}

void AGOKatnissCharacter::OnSkillW()
{
	Super::OnSkillW();
	// ProcessComboCommand();
	UE_LOG(LogTemp, Log, TEXT("Katniss Skill W is triggered."));
}

void AGOKatnissCharacter::OnSkillE()
{
	Super::OnSkillE();
	UE_LOG(LogTemp, Log, TEXT("Katniss Skill E is triggered."));
}

void AGOKatnissCharacter::OnSkillR()
{
	Super::OnSkillR();
	UE_LOG(LogTemp, Log, TEXT("Katniss Skill E is triggered."));
}
