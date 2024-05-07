// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include "GOCharacterData.h"
#include "GOCharacterStat.h"
#include "Skill/GOSkillBase.h"
#include "Skill/GOSpellBase.h"

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

	ConstructorHelpers::FObjectFinder<UDataTable> SpellDataObj(TEXT("DataTable'/Game/GameData/SpellDataTable/GOSpellDataTable.GOSpellDataTable'"));
	if (SkillStatDataObj.Succeeded())
	{
		SpellDataTable = SpellDataObj.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> SpellStatDataObj(TEXT("DataTable'/Game/GameData/SpellStatDataTable/GOSpellStatDataTable.GOSpellStatDataTable'"));
	if (SkillStatDataObj.Succeeded())
	{
		SpellStatDataTable = SpellStatDataObj.Object;
	}
}

void UGOGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetAllCharacterClassSkill();
	SetAllClassSpell();
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

FGOSpellData* UGOGameSubsystem::GetSpellData(FName InSpellName)
{
	static const FString ContextString(TEXT("Spell Data Lookup"));
	FGOSpellData* SpellDataRow = SpellDataTable->FindRow<FGOSpellData>(InSpellName, ContextString, true);
	UE_LOG(LogTemp, Warning, TEXT("GOGameInstance GetSpellData is called."));
	return SpellDataRow;
}

FGOSpellStat* UGOGameSubsystem::GetSpellStatData(FName InSpellStatName)
{
	static const FString ContextString(TEXT("Spell Stat Data Lookup"));
	FGOSpellStat* SpellStatDataRow = SpellStatDataTable->FindRow<FGOSpellStat>(InSpellStatName, ContextString, true);
	UE_LOG(LogTemp, Warning, TEXT("GOGameInstance GetCharacterSpellSet is called."));
	return SpellStatDataRow;
}

void UGOGameSubsystem::SetAllCharacterClassSkill()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>(); // 자기가 자기를 불러옴... this...

	// TODO: Enum Iterator
	for (uint8 HeroType = static_cast<uint8>(EHeroType::None) + 1; HeroType<static_cast<uint8>(EHeroType::Max); ++HeroType)
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
		const FString SkillName = GetSkillNameFromEnum(Pair.Key.SkillType);
		UE_LOG(LogTemp, Warning, TEXT("[Subsystem] HeroType: %s, SkillType: %s"), *HeroName, *SkillName);
	}
}

void UGOGameSubsystem::SetAllClassSpell()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>(); // 자기가 자기를 불러옴... this...

	//for (uint8 SpellType = static_cast<uint8>(ESpellType::None) + 1; SpellType < static_cast<uint8>(ESpellType::Max); ++SpellType)
	for (uint8 HeroType = static_cast<uint8>(EHeroType::None) + 1; HeroType < static_cast<uint8>(EHeroType::Max); ++HeroType)
	{
		//FName SpellName = GetSpellTypeFName(static_cast<ESpellType>(SpellType));
		FName HeroName = GetHeroTypeFName(static_cast<EHeroType>(HeroType));

		//FGOSpellData* SpellData = GOGameInstance->GetSpellData(SpellName);
		FGOCharacterData* CharacterData = GOGameInstance->GetCharacterData(HeroName);

		if (CharacterData != nullptr)
		{
			TMap<ECharacterSpells, FSpellInfo> SpellMap =
			{
				{ECharacterSpells::Spell01, FSpellInfo(CharacterData->Spell01Class, CharacterData->DefaultSpellName01)},
				{ECharacterSpells::Spell02, FSpellInfo(CharacterData->Spell02Class, CharacterData->DefaultSpellName02)},
				{ECharacterSpells::Spell03, FSpellInfo(CharacterData->Spell03Class, CharacterData->DefaultSpellName03)}
			};

			for (const auto& Spell : SpellMap)
			{
				UGOSpellBase* SpellInstance = NewObject<UGOSpellBase>(this, Spell.Value.SpellClass);
				if (SpellInstance)
				{
					SpellInstance->InitializeSpell(Spell.Value.SpellStatName);
					AllPlayersSpell.Add(FHeroSpellKey(static_cast<EHeroType>(HeroType), Spell.Key), SpellInstance);
				}
			}
		}

	}

	// 로그로 출력 (확인용)
	for (const auto& Pair : AllPlayersSpell)
	{
		const FString HeroName = UEnum::GetValueAsString<EHeroType>(Pair.Key.HeroType);
		const FString SpellName = GetSpellNameFromEnum(Pair.Key.SpellType);
		UE_LOG(LogTemp, Warning, TEXT("[Subsystem] HeroType: %s, SpellType: %s"), *HeroName, *SpellName);
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

TMap<ECharacterSpells, UGOSpellBase*> UGOGameSubsystem::GetCharacterSpellSet(EHeroType HeroType)
{
	TMap<ECharacterSpells, UGOSpellBase*> Result;

	// Iterate through all skills and add the ones matching the given HeroType.
	for (const auto& Pair : AllPlayersSpell)
	{
		if (Pair.Key.HeroType == HeroType)
		{
			Result.Add(Pair.Key.SpellType, Pair.Value);
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

UGOSpellBase* UGOGameSubsystem::GetSpellByHeroSpellKey(const FHeroSpellKey& Key)
{
	TObjectPtr<UGOSpellBase>* SpellInstancePtr = AllPlayersSpell.Find(Key);
	if (SpellInstancePtr != nullptr)
	{
		return SpellInstancePtr->Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No spell found for HeroType: %s, SkillType: %s"),
			*UEnum::GetValueAsString<EHeroType>(Key.HeroType),
			*GetSpellNameFromEnum(Key.SpellType));
		return nullptr;
	}
}

FString UGOGameSubsystem::GetSkillNameFromEnum(ECharacterSkills SkillType) const
{
	switch (SkillType)
	{
	case ECharacterSkills::BaseSkill:
		return TEXT("BaseSkill");
	case ECharacterSkills::Skill01:
		return TEXT("Skill01");
	case ECharacterSkills::Skill02:
		return TEXT("Skill02");
	case ECharacterSkills::Skill03:
		return TEXT("Skill03");
	case ECharacterSkills::UltimateSkill:
		return TEXT("UltimateSkill");
	default:
		return TEXT("Unknown");
	}
}

FString UGOGameSubsystem::GetSpellNameFromEnum(ECharacterSpells SpellType) const
{
	switch (SpellType)
	{
	case ECharacterSpells::Spell01:
		return TEXT("Spell01");
	case ECharacterSpells::Spell02:
		return TEXT("Spell02");
	default:
		return TEXT("Unknown");
	}
}

FName UGOGameSubsystem::GetHeroTypeFName(EHeroType HeroType)
{
	switch (HeroType)
	{
	case EHeroType::Rogers: return FName(TEXT("Rogers"));
	case EHeroType::Katniss: return FName(TEXT("Katniss"));
	case EHeroType::Beast: return FName(TEXT("Beast"));
	case EHeroType::Bride: return FName(TEXT("Bride"));
	default: return FName(TEXT("None"));
	}
}

FName UGOGameSubsystem::GetSpellTypeFName(ESpellType SpellType)
{
	switch (SpellType)
	{
	case ESpellType::GOSpell_Ghost: return FName(TEXT("GOSpell_Ghost"));
	case ESpellType::GOSpell_Heal: return FName(TEXT("GOSpell_Heal"));
	default: return FName(TEXT("None"));
	}
}
