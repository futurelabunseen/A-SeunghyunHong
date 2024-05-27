// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameData/GOCharacterStat.h"
#include "GameData/GOCharacterData.h"
#include "GameData/GOSkillStat.h"
#include "GameData/GOSkillData.h"
#include "GameData/GOSpellStat.h"
#include "GameData/GOSpellData.h"
#include "Skill/GOSkillBase.h"
#include "Skill/GOSpellBase.h"
#include "Share/ShareEnums.h"
// #include "GOSkillStat.h"
#include "UObject/UnrealNames.h"  
#include "Kismet/KismetStringLibrary.h"
#include "GOGameSubsystem.generated.h"

class UImage;

// Skill
USTRUCT(BlueprintType)
struct FSkillInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UGOSkillBase> SkillClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SkillStatName;

    FSkillInfo() : SkillClass(nullptr), SkillStatName(NAME_None) {}
    FSkillInfo(TSubclassOf<UGOSkillBase> InSkillClass, FName InSkillStatName) 
        : SkillClass(InSkillClass), SkillStatName(InSkillStatName) {}
};

USTRUCT(BlueprintType)
struct FHeroSkillKey
{
    GENERATED_BODY()

    UPROPERTY()
    EHeroType HeroType;

    UPROPERTY()
    ECharacterSkills SkillType;

    FHeroSkillKey() : HeroType(EHeroType::None), SkillType(ECharacterSkills::None) {}
    FHeroSkillKey(EHeroType InHeroType, ECharacterSkills InSkillType)
        : HeroType(InHeroType), SkillType(InSkillType) {}

    friend uint8 GetTypeHash(const FHeroSkillKey& Key)
    {
        uint8 HashCode = 0;
        HashCode = HashCombine(HashCode, GetTypeHash(Key.HeroType));
        HashCode = HashCombine(HashCode, GetTypeHash(Key.SkillType));
        return HashCode;
    }

    bool operator==(const FHeroSkillKey& Other) const
    {
        return HeroType == Other.HeroType && SkillType == Other.SkillType;
    }

    bool operator!=(const FHeroSkillKey& Other) const
    {
        return !(*this == Other);
    }
};

// Spell
USTRUCT(BlueprintType)
struct FSpellInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UGOSpellBase> SpellClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SpellStatName;

    FSpellInfo() : SpellClass(nullptr), SpellStatName(NAME_None) {}
    FSpellInfo(TSubclassOf<UGOSpellBase> InSpellClass, FName InSpellStatName)
        : SpellClass(InSpellClass), SpellStatName(InSpellStatName) {}
};

USTRUCT(BlueprintType)
struct FHeroSpellKey
{
    GENERATED_BODY()

    UPROPERTY()
    EHeroType HeroType;

    UPROPERTY()
    ECharacterSpells SpellType;

    FHeroSpellKey() : HeroType(EHeroType::None), SpellType(ECharacterSpells::None) {}
    FHeroSpellKey(EHeroType InHeroType, ECharacterSpells InSpellType)
        : HeroType(InHeroType), SpellType(InSpellType) {}

    friend uint8 GetTypeHash(const FHeroSpellKey& Key)
    {
        uint8 HashCode = 0;
        HashCode = HashCombine(HashCode, GetTypeHash(Key.HeroType));
        HashCode = HashCombine(HashCode, GetTypeHash(Key.SpellType));
        return HashCode;
    }

    bool operator==(const FHeroSpellKey& Other) const
    {
        return HeroType == Other.HeroType && SpellType == Other.SpellType;
    }

    bool operator!=(const FHeroSpellKey& Other) const
    {
        return !(*this == Other);
    }
};

USTRUCT(BlueprintType)
struct FHeroSelectionData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FHeroSelectionInfo> RedTeamHeroes;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FHeroSelectionInfo> BlueTeamHeroes;
};

UCLASS()
class GUARDIANSORDERS_API UGOGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    UGOGameSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    UGOGameSubsystem* GetGOGameSubsystem();

    // 기능 접근 메서드
    FGOCharacterStat GetCharacterStat(int32 InCType) const { return CharacterStatTable.IsValidIndex(InCType - 1) ? CharacterStatTable[InCType - 1] : FGOCharacterStat(); }
    //FORCEINLINE FGOSkillStat GetSkillStat(int32 InSType) const { return SkillStatTable.IsValidIndex(InSType - 1) ? SkillStatTable[InSType - 1] : FGOSkillStat(); }
    
    // For Newer Game data Setting System
    FGOCharacterData* GetCharacterData(FName InCharacterName);
    FGOCharacterStat* GetCharacterStatData(FName InCharacterName);    
    FGOSkillData* GetSkillData(FName InSkillName);
    FGOSkillStat* GetSkillStatData(FName InSkillStatName);

    FGOSpellData* GetSpellData(FName InSpellName);
    FGOSpellStat* GetSpellStatData(FName InSpellStatName);

    void SetAllCharacterClassSkill();

    void SetAllClassSpell();
    
    TMap<ECharacterSkills, UGOSkillBase*> GetCharacterSkillSet(EHeroType HeroType);
    TMap<ECharacterSpells, UGOSpellBase*> GetCharacterSpellSet(EHeroType HeroType);
    
    UGOSkillBase* GetSkillByHeroSkillKey(const FHeroSkillKey& Key);
    UGOSpellBase* GetSpellByHeroSpellKey(const FHeroSpellKey& Key);

    FString GetSkillNameFromEnum(ECharacterSkills SkillType) const;
    FString GetSpellNameFromEnum(ECharacterSpells SpellType) const;

    FName GetHeroTypeFName(EHeroType HeroType);

    FName GetSpellTypeFName(ESpellType SpellType);

    UTexture2D* GetHeroImageByEHeroType(EHeroType HeroType);
    // void GetHeroImageByEHeroType(EHeroType HeroType, UTexture2D*& OutTexture);

    TSubclassOf<class AGOPlayerCharacter> GetCharacterClassByHeroType(EHeroType HeroType) const;

    void SetHeroSelectionData(const FHeroSelectionData& Data);
    FHeroSelectionData GetHeroSelectionData() const;
protected:
    void InitializeHeroCharacterMap();

    TArray<FGOCharacterStat> CharacterStatTable;

    // For Newer Game data Setting System
    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> CharacterDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> CharacterStatDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> SkillDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> SkillStatDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> SpellDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> SpellStatDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Character")
    TObjectPtr<UDataTable> HeroCharacterDataTable;

    //Skill
    UPROPERTY(EditDefaultsOnly, Category = "Skills")
    TMap<FHeroSkillKey, TObjectPtr<UGOSkillBase>> AllPlayersSkill;

    //Spell
    UPROPERTY(EditDefaultsOnly, Category = "Spells")
    TMap<FHeroSpellKey, TObjectPtr<UGOSpellBase>> AllPlayersSpell;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character")
    TMap<EHeroType, TSubclassOf<class AGOPlayerCharacter>> HeroCharacterMap;

    FHeroSelectionData HeroSelectionData;

public:
    int32 CharacterMaxCnt;
};
