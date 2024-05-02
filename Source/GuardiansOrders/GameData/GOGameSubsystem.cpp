// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>

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
}

void UGOGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetAllCharacterClassSkill();
}

void UGOGameSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

UGOGameSubsystem* UGOGameSubsystem::GetGOGameSubsystem()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is not available."));
		return nullptr;
	}

	UGOGameSubsystem* GOGameSubsystem = GameInstance->GetSubsystem<UGOGameSubsystem>();
	if (!ensure(GOGameSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGOGameSubsystem is not available."));
		return nullptr;
	}

	return GOGameSubsystem;
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

void UGOGameSubsystem::SetAllCharacterClassSkill()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	for (int32 HeroType = static_cast<int32>(EHeroType::None) + 1; HeroType<static_cast<int32>(EHeroType::Max); ++HeroType)
	{
		FName HeroName = GetHeroTypeFName(static_cast<EHeroType>(HeroType));
		FGOCharacterData* CharacterData = GOGameInstance->GetCharacterData(HeroName);

		if (CharacterData != nullptr)
		{
			TMap<ECharacterSkills, FSkillInfo> SkillMap =
			{
				{ECharacterSkills::BaseSkill, FSkillInfo(CharacterData->BaseSkillClass, CharacterData->DefaultBaseSkillName)},
				{ECharacterSkills::Skill01, FSkillInfo(CharacterData->SkillQClass, CharacterData->DefaultSkillNameQ)},
				{ECharacterSkills::Skill02, FSkillInfo(CharacterData->SkillWClass, CharacterData->DefaultSkillNameW)},
				{ECharacterSkills::Skill03, FSkillInfo(CharacterData->SkillEClass, CharacterData->DefaultSkillNameE)},
				{ECharacterSkills::UltimateSkill, FSkillInfo(CharacterData->SkillRClass, CharacterData->DefaultSkillNameR)}
			};

			for (const auto& Skill : SkillMap)
			{
				UGOSkillBase* SkillInstance = NewObject<UGOSkillBase>(this, Skill.Value.SkillClass);
				if (SkillInstance)
				{
					SkillInstance->InitializeSkill(Skill.Value.SkillStatName);
					AllPlayersSkill.Add(FHeroSkillKey(static_cast<EHeroType>(HeroType), Skill.Key), SkillInstance);
				}
			}
		}
	}

	// 로그로 출력 (확인용)
	for (const auto& Pair : AllPlayersSkill)
	{
		const FString HeroName = UEnum::GetValueAsString<EHeroType>(Pair.Key.HeroType);
		const FString SkillName = GetSkillNameFromEnum(Pair.Key.SkillType); // 적절한 함수를 호출하여 Enum을 문자열로 변환
		UE_LOG(LogTemp, Warning, TEXT("[Subsystem] HeroType: %s, SkillType: %s"), *HeroName, *SkillName);
	}
}

TMap<ECharacterSkills, UGOSkillBase*> UGOGameSubsystem::GetCharacterSkillSet(EHeroType HeroType)
{
	TMap<ECharacterSkills, UGOSkillBase*> Result;

	// Iterate through all skills and add the ones matching the given HeroType.
	for (const auto& Pair : AllPlayersSkill)
	{
		if (Pair.Key.HeroType == HeroType)
		{
			Result.Add(Pair.Key.SkillType, Pair.Value);
		}
	}

	return Result;
}

UGOSkillBase* UGOGameSubsystem::GetSkillByHeroSkillKey(const FHeroSkillKey& Key)
{
	TObjectPtr<UGOSkillBase>* SkillInstancePtr = AllPlayersSkill.Find(Key);
	if (SkillInstancePtr != nullptr)
	{
		return SkillInstancePtr->Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No skill found for HeroType: %s, SkillType: %s"),
			*UEnum::GetValueAsString<EHeroType>(Key.HeroType),
			*GetSkillNameFromEnum(Key.SkillType));
		return nullptr;
	}
}
