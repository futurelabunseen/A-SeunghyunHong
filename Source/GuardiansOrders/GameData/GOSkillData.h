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
        SkillName(""),
        Description(""),
        SkillTriggerType(ESkillTriggerType::None),
        SkillAffectType(ESkillAffectType::None),
        SkillAnim(nullptr),
        SkillEffect(nullptr),
        SkillIcon(nullptr),
        CameraShakeIntensity(1.0f)
    {}

    // 스킬 스탯을 위한 테이블 RowName 입니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    FName SkillStatName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SkillData")
    FString SkillName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SkillData")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    ESkillTriggerType SkillTriggerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    ESkillAffectType SkillAffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    ESkillCastType SkillCastType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    EParticleSpawnLocation ParticleSpawnLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UAnimMontage> SkillAnim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UParticleSystem> SkillEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UTexture2D> SkillIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    float CameraShakeIntensity;
};

