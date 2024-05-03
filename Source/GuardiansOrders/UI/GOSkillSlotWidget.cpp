// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOSkillSlotWidget.h"

void UGOSkillSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CooldownImage)
    {
        UMaterialInterface* BaseMaterial = CooldownImage->GetBrush().GetResourceObject()->GetClass()->GetDefaultObject<UMaterialInterface>();
        if (BaseMaterial)
        {
            MatInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
            CooldownImage->SetBrushFromMaterial(MatInstance);
        }
    }
}

void UGOSkillSlotWidget::BindSkill(UGOSkillBase* Skill)
{
    if (!Skill)
    {
        return;
    }
        
    SkillIconImage->SetBrushFromTexture(Skill->GetTotalSkillData().SkillIcon); // ��ų ������ ����
    // CooldownText
    UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI BindSkill] SkillIconImage is %s"), *Skill->GetTotalSkillData().SkillIcon.GetName());

    // Skill->OnCooldownUpdated.AddDynamic(this, &UGOSkillSlotWidget::UpdateCooldownDisplay); // ��ٿ� ������Ʈ �̺�Ʈ ���ε�
    
}
