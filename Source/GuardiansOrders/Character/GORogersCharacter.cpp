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
		// ManaRegenerationRate = Stat->GetTotalStat().ManaRegenerationRate;
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