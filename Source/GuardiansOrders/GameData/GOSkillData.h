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
    
    // ��ų ������ ���� ���̺� RowName �Դϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    FName SkillStatName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    ESkillTriggerType SkillTriggerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    ESkillAffectType SkillAffectType;

    // ��ų �ִϸ��̼��� ���� �����Դϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UAnimMontage> SkillAnim;

    // ��ų ȿ�� �ּ��� ���� �����Դϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UGOSkillEffectAsset> SkillEffectAsset;

    // ��ų �������� �ؽ����Դϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
    TObjectPtr<UTexture2D> SkillIcon;

    // �߰����� ��� ������...
};

