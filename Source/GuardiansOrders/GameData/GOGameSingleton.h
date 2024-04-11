// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GOCharacterStat.h"
#include "GOSkillStat.h"
#include "GOGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGOGameSingleton, Error, All);

UCLASS()
class GUARDIANSORDERS_API UGOGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UGOGameSingleton();
	
	static UGOGameSingleton& Get();

// Character Stat Data
public:
	FORCEINLINE FGOCharacterStat GetCharacterStat(int32 InCType) const { return CharacterStatTable.IsValidIndex(InCType - 1) ? CharacterStatTable[InCType - 1] : FGOCharacterStat(); }
	FORCEINLINE FGOSkillStat GetSkillStat(int32 InSType) const { return SkillStatTable.IsValidIndex(InSType - 1) ? SkillStatTable[InSType - 1] : FGOSkillStat(); }

	UPROPERTY()
	int32 CharacterMaxCnt;

	UPROPERTY()
	int32 SkillMaxCnt;

private:
	TArray<FGOCharacterStat> CharacterStatTable;
	TArray<FGOSkillStat> SkillStatTable;
};
