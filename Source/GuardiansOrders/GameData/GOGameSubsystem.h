// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameData/GOCharacterStat.h"
#include "GameData/GOCharacterData.h"
#include "GameData/GOSkillStat.h"
#include "GameData/GOSkillData.h"
#include "Skill/GOSkillBase.h"
#include "Share/ShareEnums.h"
#include "GOSkillStat.h"
#include "UObject/UnrealNames.h"  
#include "Kismet/KismetStringLibrary.h"
#include "GOGameSubsystem.generated.h"

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
    FORCEINLINE FGOCharacterStat GetCharacterStat(int32 InCType) const { return CharacterStatTable.IsValidIndex(InCType - 1) ? CharacterStatTable[InCType - 1] : FGOCharacterStat(); }
    //FORCEINLINE FGOSkillStat GetSkillStat(int32 InSType) const { return SkillStatTable.IsValidIndex(InSType - 1) ? SkillStatTable[InSType - 1] : FGOSkillStat(); }
    
    // For Newer Game data Setting System
    FORCEINLINE FGOCharacterData* GetCharacterData(FName InCharacterName);
    FORCEINLINE FGOCharacterStat* GetCharacterStatData(FName InCharacterName);    
    FORCEINLINE FGOSkillData* GetSkillData(FName InSkillName);
    FORCEINLINE FGOSkillStat* GetSkillStatData(FName InSkillStatName);

    void SetAllCharacterClassSkill();
    
    TMap<ECharacterSkills, UGOSkillBase*> GetCharacterSkillSet(EHeroType HeroType);
    
    UGOSkillBase* GetSkillByHeroSkillKey(const FHeroSkillKey& Key);

    FString GetSkillNameFromEnum(ECharacterSkills SkillType) const
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

    FName GetHeroTypeFName(EHeroType HeroType)
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


protected:
    TArray<FGOCharacterStat> CharacterStatTable;
    //TArray<FGOSkillStat> SkillStatTable;

    // For Newer Game data Setting System
    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> CharacterDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> CharacterStatDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> SkillDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> SkillStatDataTable;

    //UPROPERTY()
    TMap<FHeroSkillKey, FSkillInfo> SkillClassTypeData;

    //UPROPERTY(VisibleAnywhere, Category = "Skills")
    //TMap<EHeroType, TMap<ECharacterSkills, TObjectPtr<UGOSkillBase>>> AllPlayersSkill;
    UPROPERTY(EditDefaultsOnly, Category = "Skills")
    TMap<FHeroSkillKey, TObjectPtr<UGOSkillBase>> AllPlayersSkill;

public:
    int32 CharacterMaxCnt;
};
