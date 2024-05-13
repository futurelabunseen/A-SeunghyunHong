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

    // ��ų ������ ���� ���̺� RowName �Դϴ�.
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

    // ��ų ȿ�� �ּ��� ���� �����Դϴ�. (����: �ּ� �Ǵ� ���̺�)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    TObjectPtr<UGOSkillEffectAsset> SkillEffectAsset;

    // ��ų �������� �ؽ����Դϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    TObjectPtr<UTexture2D> SpellIcon;
};

