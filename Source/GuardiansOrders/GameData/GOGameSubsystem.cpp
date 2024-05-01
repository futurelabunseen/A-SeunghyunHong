// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/GOGameSubsystem.h"

UGOGameSubsystem::UGOGameSubsystem()
{
	ConstructorHelpers::FObjectFinder<UDataTable> CharacterDataObj(TEXT("DataTable'/Game/GameData/CharacterDataTable/GOCharacterDataTable.GOCharacterDataTable'"));
	if (CharacterDataObj.Succeeded())
	{
		CharacterDataTable = CharacterDataObj.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> CharacterStatDataObj(TEXT("DataTable'/Game/GameData/CharacterStatDataTable/GOCharacterStatTable.GOCharacterStatTable'"));
	if (CharacterStatDataObj.Succeeded())
	{
		CharacterStatDataTable = CharacterStatDataObj.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> SkillDataObj(TEXT("DataTable'/Game/GameData/SkillDataTable/GOSkillDataTable.GOSkillDataTable'"));
	if (SkillDataObj.Succeeded())
	{
		SkillDataTable = SkillDataObj.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> SkillStatDataObj(TEXT("DataTable'/Game/GameData/SkillStatDataTable/GOSkillStatDataTable.GOSkillStatDataTable'"));
	if (SkillStatDataObj.Succeeded())
	{
		SkillStatDataTable = SkillStatDataObj.Object;
	}

	//CharacterMaxCnt = 0;
	//SkillMaxCnt = 0;

	// Character stat
	//static ConstructorHelpers::FObjectFinder<UDataTable> CharacterStatDataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/HeroStatDataTable/GOCharacterStatTable.GOCharacterStatTable'"));
	//if (nullptr != CharacterStatDataTableRef.Object)
	//{
	//	const UDataTable* DataTable = CharacterStatDataTableRef.Object;
	//	check(DataTable->GetRowMap().Num() > 0);

	//	TArray<uint8*> ValueArray;
	//	DataTable->GetRowMap().GenerateValueArray(ValueArray);
	//	Algo::Transform(ValueArray, CharacterStatTable,
	//		[](uint8* Value)
	//		{
	//			return *reinterpret_cast<FGOCharacterStat*>(Value);
	//		}
	//	);
	//}

	//CharacterMaxCnt = CharacterStatTable.Num();
	//ensure(CharacterMaxCnt > 0);

	// Skill stat
	//static ConstructorHelpers::FObjectFinder<UDataTable> SkillStatDataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/SkillStatDataTable/GOSkillStatDataTable.GOSkillStatDataTable'"));
	//if (nullptr != SkillStatDataTableRef.Object)
	//{
	//	const UDataTable* DataTable = SkillStatDataTableRef.Object;
	//	check(DataTable->GetRowMap().Num() > 0);

	//	TArray<uint8*> ValueArray;
	//	DataTable->GetRowMap().GenerateValueArray(ValueArray);
	//	Algo::Transform(ValueArray, SkillStatTable,
	//		[](uint8* Value)
	//		{
	//			return *reinterpret_cast<FGOSkillStat*>(Value);
	//		}
	//	);
	//}

	//SkillMaxCnt = SkillStatTable.Num();
	//ensure(SkillMaxCnt > 0);
}

void UGOGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UGOGameSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FGOCharacterData* UGOGameSubsystem::GetCharacterData(FName InCharacterName)
{
	static const FString ContextString(TEXT("Character Data Lookup"));
	FGOCharacterData* CharacterDataRow = CharacterDataTable->FindRow<FGOCharacterData>(InCharacterName, ContextString, true);
	UE_LOG(LogTemp, Warning, TEXT("GOGameInstance GetCharacterData is called."));
	return CharacterDataRow;
}

FGOCharacterStat* UGOGameSubsystem::GetCharacterStatData(FName InCharacterName)
{
	static const FString ContextString(TEXT("Character Stat Data Lookup"));
	FGOCharacterStat* CharacterStatDataRow = CharacterStatDataTable->FindRow<FGOCharacterStat>(InCharacterName, ContextString, true);
	UE_LOG(LogTemp, Warning, TEXT("GOGameInstance GetCharacterStatData is called."));
	return CharacterStatDataRow;
}

FGOSkillData* UGOGameSubsystem::GetSkillData(FName InSkillName)
{
	static const FString ContextString(TEXT("Skill Data Lookup"));
	FGOSkillData* SkillDataRow = SkillDataTable->FindRow<FGOSkillData>(InSkillName, ContextString, true);
	UE_LOG(LogTemp, Warning, TEXT("GOGameInstance GetSkillData is called."));
	return SkillDataRow;
}

FGOSkillStat* UGOGameSubsystem::GetSkillStatData(FName InSkillStatName)
{
	static const FString ContextString(TEXT("Skill Stat Data Lookup"));
	FGOSkillStat* SkillStatDataRow = SkillStatDataTable->FindRow<FGOSkillStat>(InSkillStatName, ContextString, true);
	UE_LOG(LogTemp, Warning, TEXT("GOGameInstance GetSkillStatData is called."));
	return SkillStatDataRow;
}