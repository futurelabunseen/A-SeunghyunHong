// Fill out your copyright notice in the Description page of Project Settings.


#include "GOBrideCharacter.h"
#include "CharacterStat/GOCharacterStatComponent.h"

AGOBrideCharacter::AGOBrideCharacter()
{
	bReplicates = true;

	//static ConstructorHelpers::FObjectFinder<UGOCharacterDataAsset> DataAsset(TEXT("/Game/GameData/HeroDataAsset/GOHero_Bride.GOHero_Bride"));
	//if (DataAsset.Succeeded())
	//{
	//	HeroDataAsset = DataAsset.Object;
	//	Stat->SetCurrentCharacterType(static_cast<int32>(EHeroType::Bride));
	//	// ManaRegenerationRate = Stat->GetTotalStat().ManaRegenerationRate;
	//}
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

void AGOBrideCharacter::OnSkillQ()
{
	UE_LOG(LogTemp, Log, TEXT("Bride Skill Q is triggered."));
}

void AGOBrideCharacter::OnSkillW()
{
	// ProcessComboCommand();
	UE_LOG(LogTemp, Log, TEXT("Bride Skill W is triggered."));
}

void AGOBrideCharacter::OnSkillE()
{
	UE_LOG(LogTemp, Log, TEXT("Bride Skill E is triggered."));
}

void AGOBrideCharacter::OnSkillR()
{
	UE_LOG(LogTemp, Log, TEXT("Bride Skill R is triggered."));
}



