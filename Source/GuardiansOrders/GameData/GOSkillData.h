#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Share/EGOSkill.h"
#include "GOSkillEffectAsset.h"
#include "GOSkillData.generated.h"


// #include "Animation/AnimMontage.h"

USTRUCT(BlueprintType)
struct FGOSkillData : public FTableRowBase
{
    GENERATED_BODY()

    public:
    FGOSkillData() :
        SkillStatName(NAME_None),
        SkillAnim(nullptr),
        SkillEffectAsset(nullptr),
        SkillIcon(nullptr),
        SkillTriggerType(ESkillTriggerType::None), 
        SkillAffectType(ESkillAffectType::None)
        {}
    
    // 스킬 스탯을 위한 테이블 RowName 입니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    FName SkillStatName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    ESkillTriggerType SkillTriggerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    ESkillAffectType SkillAffectType;

    // 스킬 애니메이션을 위한 참조입니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UAnimMontage> SkillAnim;

    // 스킬 효과 애셋을 위한 참조입니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UGOSkillEffectAsset> SkillEffectAsset;

    // 스킬 아이콘의 텍스쳐입니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UTexture2D> SkillIcon;

    // 추가적인 멤버 변수들...
};

