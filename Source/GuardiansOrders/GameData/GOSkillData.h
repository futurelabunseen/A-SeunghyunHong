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
        SkillEffectAsset(nullptr),
        SkillIcon(nullptr)
    {}

    // ��ų ������ ���� ���̺� RowName �Դϴ�.
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
    TObjectPtr<UAnimMontage> SkillAnim;

    // ��ų ȿ�� �ּ��� ���� �����Դϴ�. (����: �ּ� �Ǵ� ���̺�)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UGOSkillEffectAsset> SkillEffectAsset;

    // ��ų �������� �ؽ����Դϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UTexture2D> SkillIcon;
};

