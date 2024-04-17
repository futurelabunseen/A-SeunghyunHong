// Fill out your copyright notice in the Description page of Project Settings.


#include "GORogersCharacter.h"
#include "CharacterStat/GOCharacterStatComponent.h"

AGORogersCharacter::AGORogersCharacter()
{
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UGOCharacterDataAsset> DataAsset(TEXT("/Game/GameData/HeroDataAsset/GOHero_Rogers.GOHero_Rogers"));
	if (DataAsset.Succeeded())
	{
		HeroDataAsset = DataAsset.Object;
		Stat->SetCurrentCharacterType(static_cast<int32>(EHeroType::Rogers));
	}

	// GORogersSkill01 객체 생성
	GORogersSkill01 = CreateDefaultSubobject<UGORogersSkill01>(TEXT("GORogersSkill01"));
	if (!GORogersSkill01)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create GORogersSkill01"));
	}

	// GORogersSkill02 객체 생성
	GORogersSkill02 = CreateDefaultSubobject<UGORogersSkill02>(TEXT("GORogersSkill02"));
	if (!GORogersSkill02)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create GORogersSkill02"));
	}

	// GORogersSkill03 객체 생성
	GORogersSkill03 = CreateDefaultSubobject<UGORogersSkill03>(TEXT("GORogersSkill03"));
	if (!GORogersSkill03)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create GORogersSkill03"));
	}

	// GORogersUltimateSkill 객체 생성
	GORogersUltimateSkill = CreateDefaultSubobject<UGORogersUltimateSkill>(TEXT("GORogersUltimateSkill"));
	if (!GORogersUltimateSkill)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create GORogersUltimateSkill"));
	}
}

void AGORogersCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AGORogersCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGORogersCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GORogersSkill01)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillStat GORogersSkill01 Name: %s"), *GORogersSkill01->Name);
		UE_LOG(LogTemp, Warning, TEXT("SkillStat GORogersSkill01 DamageMultiplier: %f"), GORogersSkill01->DamageMultiplier);
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