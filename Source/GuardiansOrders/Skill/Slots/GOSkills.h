// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Share/EGOSkill.h"
#include "Skill/GOSkillBase.h"
#include "GOSkills.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSkillInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UGOSkillBase> SkillClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SkillStatName;

    FSkillInfo() : SkillClass(nullptr), SkillStatName(NAME_None) {}
    FSkillInfo(TSubclassOf<UGOSkillBase> InSkillClass, FName InSkillStatName) : SkillClass(InSkillClass), SkillStatName(InSkillStatName) {}
};

UCLASS()
class GUARDIANSORDERS_API UGOSkills : public UObject
{
	GENERATED_BODY()
	
public:
    UGOSkills();

    UFUNCTION(BlueprintCallable, Category = "Skills")
    void InitializeSkills(FName CharacterName);

    UFUNCTION(BlueprintCallable, Category = "Skills")
    UGOSkillBase* GetSkill(ECharacterSkills SkillType);

private:
    UPROPERTY()
    TMap<ECharacterSkills, FSkillInfo> SkillClassTypeData;

    UPROPERTY(VisibleAnywhere, Category = "Skills")
    TMap<ECharacterSkills, UGOSkillBase*> Skills;
};
