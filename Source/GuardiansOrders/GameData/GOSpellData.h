#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Share/EGOSkill.h"
#include "GOSkillEffectAsset.h"
#include "GOSpellData.generated.h"


// #include "Animation/AnimMontage.h"

USTRUCT(BlueprintType)
struct FGOSpellData : public FTableRowBase
{
    GENERATED_BODY()

 public:
    FGOSpellData() :
        SpellStatName(NAME_None),
        SpellName(""),
        Description(""),
        SpellTriggerType(ESkillTriggerType::None),
        SpellAffectType(ESkillAffectType::None),
        SpellAnim(nullptr),
        SkillEffectAsset(nullptr),
        SpellIcon(nullptr)
    {}

    // 스킬 스탯을 위한 테이블 RowName 입니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    FName SpellStatName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpellData")
    FString SpellName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpellData")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    ESkillTriggerType SpellTriggerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    ESkillAffectType SpellAffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    TObjectPtr<UAnimMontage> SpellAnim;

    // 스킬 효과 애셋을 위한 참조입니다. (미정: 애셋 또는 테이블)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    TObjectPtr<UGOSkillEffectAsset> SkillEffectAsset;

    // 스킬 아이콘의 텍스쳐입니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    TObjectPtr<UTexture2D> SpellIcon;
};

