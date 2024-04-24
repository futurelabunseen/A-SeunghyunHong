// Fill out your copyright notice in the Description page of Project Settings.


#include "GOKatnissCharacter.h"
#include "CharacterStat/GOCharacterStatComponent.h"

AGOKatnissCharacter::AGOKatnissCharacter()
{
	bReplicates = true;

	//static ConstructorHelpers::FObjectFinder<UGOCharacterDataAsset> DataAsset(TEXT("/Game/GameData/HeroDataAsset/GOHero_Katniss.GOHero_Katniss"));
	//if (DataAsset.Succeeded())
	//{
	//	HeroDataAsset = DataAsset.Object;
	//	Stat->SetCurrentCharacterType(static_cast<int32>(EHeroType::Katniss));
	//	// ManaRegenerationRate = Stat->GetTotalStat().ManaRegenerationRate;
	//}
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

void AGOKatnissCharacter::OnSkillQ()
{
}

void AGOKatnissCharacter::OnSkillW()
{
	// ProcessComboCommand();
	UE_LOG(LogTemp, Log, TEXT("Katniss Skill W is triggered."));
}

void AGOKatnissCharacter::OnSkillE()
{
	UE_LOG(LogTemp, Log, TEXT("Katniss Skill E is triggered."));
}

void AGOKatnissCharacter::OnSkillR()
{
	UE_LOG(LogTemp, Log, TEXT("Katniss Skill E is triggered."));
}
