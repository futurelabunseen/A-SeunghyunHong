// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameData/GOCharacterStat.h"
#include "GameData/GOCharacterData.h"
#include "GameData/GOSkillStat.h"
#include "GameData/GOSkillData.h"
#include "GOSkillStat.h"
#include "GOGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    UGOGameSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // 기능 접근 메서드
    FORCEINLINE FGOCharacterStat GetCharacterStat(int32 InCType) const { return CharacterStatTable.IsValidIndex(InCType - 1) ? CharacterStatTable[InCType - 1] : FGOCharacterStat(); }
    //FORCEINLINE FGOSkillStat GetSkillStat(int32 InSType) const { return SkillStatTable.IsValidIndex(InSType - 1) ? SkillStatTable[InSType - 1] : FGOSkillStat(); }
    
    // For Newer Game data Setting System
    FORCEINLINE FGOCharacterData* GetCharacterData(FName InCharacterName);
    FORCEINLINE FGOCharacterStat* GetCharacterStatData(FName InCharacterName);    
    FORCEINLINE FGOSkillData* GetSkillData(FName InSkillName);
    FORCEINLINE FGOSkillStat* GetSkillStatData(FName InSkillStatName);

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

public:
    int32 CharacterMaxCnt;
    //int32 SkillMaxCnt;

private: 
};
