// Fill out your copyright notice in the Description page of Project Settings.


#include "GOBeastCharacter.h"
#include "CharacterStat/GOCharacterStatComponent.h"

AGOBeastCharacter::AGOBeastCharacter()
{
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UGOCharacterDataAsset> DataAsset(TEXT("/Game/GameData/HeroDataAsset/GOHero_Beast.GOHero_Beast"));
	if (DataAsset.Succeeded())
	{
		HeroDataAsset = DataAsset.Object;
		Stat->SetCurrentCharacterType(static_cast<int32>(EHeroType::Beast));
		// ManaRegenerationRate = Stat->GetTotalStat().ManaRegenerationRate;
	}
}

void AGOBeastCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AGOBeastCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGOBeastCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGOBeastCharacter::OnSkillQ()
{
	UE_LOG(LogTemp, Log, TEXT("Beast Skill Q is triggered."));
}

void AGOBeastCharacter::OnSkillW()
{
	// ProcessComboCommand();
	UE_LOG(LogTemp, Log, TEXT("Beast Skill W is triggered."));
}

void AGOBeastCharacter::OnSkillE()
{
	UE_LOG(LogTemp, Log, TEXT("Beast Skill E is triggered."));
}

void AGOBeastCharacter::OnSkillR()
{
	UE_LOG(LogTemp, Log, TEXT("Beast Skill R is triggered."));
}

