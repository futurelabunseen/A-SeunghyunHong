// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/GOGameSubsystem.h"

UGOGameSubsystem::UGOGameSubsystem()
{
	CharacterMaxCnt = 0;
	SkillMaxCnt = 0;
}

void UGOGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadCharacterStats();
	LoadSkillStats();
}

void UGOGameSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UGOGameSubsystem::LoadCharacterStats()
{
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/GameData/HeroDataTable/GOCharacterStatTable.GOCharacterStatTable")));
	if (DataTable)
	{
		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable, [](uint8* Value)
			{
				return *reinterpret_cast<FGOCharacterStat*>(Value);
			});
	}
}

void UGOGameSubsystem::LoadSkillStats()
{
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/GameData/SkillDataTable/GOSkillStatTable.GOSkillStatTable")));
	if (DataTable)
	{
		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, SkillStatTable, [](uint8* Value)
			{
				return *reinterpret_cast<FGOSkillStat*>(Value);
			});
	}
}