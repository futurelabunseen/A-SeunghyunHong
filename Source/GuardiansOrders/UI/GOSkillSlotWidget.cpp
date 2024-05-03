// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOSkillSlotWidget.h"

void UGOSkillSlotWidget::BindSkill(UGOSkillBase* Skill)
{
    if (!Skill)
    {
        return;
    }
        
    SkillIconImage->SetBrushFromTexture(Skill->GetTotalSkillData().SkillIcon); // 스킬 아이콘 설정
    UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI BindSkill] SkillIconImage is %s"), *Skill->GetTotalSkillData().SkillIcon.GetName());

    // Skill->OnCooldownUpdated.AddDynamic(this, &UGOSkillSlotWidget::UpdateCooldownDisplay); // 쿨다운 업데이트 이벤트 바인딩
    
}
