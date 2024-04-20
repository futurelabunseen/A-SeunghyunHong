// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/GOGameSingleton.h"

DEFINE_LOG_CATEGORY(LogGOGameSingleton);

UGOGameSingleton::UGOGameSingleton()
{
	//// Character stat
	//static ConstructorHelpers::FObjectFinder<UDataTable> CharacterStatDataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/HeroDataTable/GOCharacterStatTable.GOCharacterStatTable'"));
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

	//// Skill stat
	//static ConstructorHelpers::FObjectFinder<UDataTable> SkillStatDataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/SkillDataTable/GOSkillStatDataTable.GOSkillStatDataTable'"));
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

UGOGameSingleton& UGOGameSingleton::Get()
{
	UGOGameSingleton* Singleton = CastChecked<UGOGameSingleton>(GEngine->GameSingleton);
	if (Singleton) {
		return *Singleton;
	}

	UE_LOG(LogGOGameSingleton, Error, TEXT("Invalid Game Singleton"));

	return *NewObject<UGOGameSingleton>();
}
