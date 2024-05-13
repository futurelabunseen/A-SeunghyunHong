// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Share/EGOSkill.h"
#include "Skill/GOSkillBase.h"
#include "GOSkills.generated.h"


UCLASS()
class GUARDIANSORDERS_API UGOSkills : public UObject
{
	GENERATED_BODY()
	
public:
    UGOSkills();

    FORCEINLINE TMap < ECharacterSkills, UGOSkillBase*> GetSkills() { return Skills; }

    UFUNCTION(BlueprintCallable, Category = "Skills")
    void InitializeSkills(FName CharacterName);

    UFUNCTION(BlueprintCallable, Category = "Skills")
    UGOSkillBase* GetSkill(ECharacterSkills SkillType);

    EHeroType ConvertFNameToHeroType(const FName& InCharacterName)
    {
        FString NameString = InCharacterName.ToString();

        // Optionally, convert to lowercase or perform other normalizations
        NameString = NameString.ToLower();

        if (NameString == TEXT("rogers"))
            return EHeroType::Rogers;
        else if (NameString == TEXT("katniss"))
            return EHeroType::Katniss;
        else if (NameString == TEXT("beast"))
            return EHeroType::Beast;
        else if (NameString == TEXT("bride"))
            return EHeroType::Bride;
        else
            return EHeroType::None; // Default case
    }
private:
   /* UPROPERTY()
    TMap<ECharacterSkills, FSkillInfo> SkillClassTypeData;*/

    UPROPERTY(VisibleAnywhere, Category = "Skills")
    TMap<ECharacterSkills, UGOSkillBase*> Skills;
};
