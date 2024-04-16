// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GOCharacterStat.h"
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
    FORCEINLINE FGOSkillStat GetSkillStat(int32 InSType) const { return SkillStatTable.IsValidIndex(InSType - 1) ? SkillStatTable[InSType - 1] : FGOSkillStat(); }

protected:
    // 데이터 테이블 로드
    void LoadCharacterStats();
    void LoadSkillStats();

    TArray<FGOCharacterStat> CharacterStatTable;
    TArray<FGOSkillStat> SkillStatTable;

    int32 CharacterMaxCnt;
    int32 SkillMaxCnt;
};
