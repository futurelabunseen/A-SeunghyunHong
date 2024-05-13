// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Share/EGOSkill.h"
#include "Skill/GOSpellBase.h"
#include "Share/ShareEnums.h"
#include "GOSpells.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOSpells : public UObject
{
	GENERATED_BODY()

public:
    UGOSpells();

    FORCEINLINE TMap < ECharacterSpells, UGOSpellBase*> GetSpells() { return Spells; }

    UFUNCTION(BlueprintCallable, Category = "Spells")
    void InitializeSpells(FName InCharacterName);

    UFUNCTION(BlueprintCallable, Category = "Spells")
    UGOSpellBase* GetSpell(ECharacterSpells SpellType);

    EHeroType ConvertFNameToHeroTypes(const FName& InCharacterName) const;


private:
    /* UPROPERTY()
     TMap<ECharacterSkills, FSkillInfo> SkillClassTypeData;*/

    UPROPERTY(VisibleAnywhere, Category = "Spells")
    TMap<ECharacterSpells, UGOSpellBase*> Spells;
};
