// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOSkillSetBarWidget.h"

void UGOSkillSetBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Ω∫≈≥ ΩΩ∑‘ ¿ß¡¨µÈ¿ª √£æ∆º≠ SkillSlotLookup ∏ ø° √ﬂ∞°
    UGOSkillSlotWidget* BaseSkillSlot = Cast<UGOSkillSlotWidget>(GetWidgetFromName(TEXT("CUI_SkillSlot_BaseSkill")));
    UGOSkillSlotWidget* Skill01Slot = Cast<UGOSkillSlotWidget>(GetWidgetFromName(TEXT("CUI_SkillSlot_Skill01")));
    UGOSkillSlotWidget* Skill02Slot = Cast<UGOSkillSlotWidget>(GetWidgetFromName(TEXT("CUI_SkillSlot_Skill02")));
    UGOSkillSlotWidget* Skill03Slot = Cast<UGOSkillSlotWidget>(GetWidgetFromName(TEXT("CUI_SkillSlot_Skill03")));
    UGOSkillSlotWidget* UltimateSkillSlot = Cast<UGOSkillSlotWidget>(GetWidgetFromName(TEXT("CUI_SkillSlot_UltimateSkill")));

    // ∞¢ ΩΩ∑‘¿ª ∏ ø° √ﬂ∞°
    if (BaseSkillSlot) SkillSlotLookup.Add(TEXT("CUI_SkillSlot_BaseSkill"), BaseSkillSlot);
    if (Skill01Slot) SkillSlotLookup.Add(TEXT("CUI_SkillSlot_Skill01"), Skill01Slot);
    if (Skill02Slot) SkillSlotLookup.Add(TEXT("CUI_SkillSlot_Skill02"), Skill02Slot);
    if (Skill03Slot) SkillSlotLookup.Add(TEXT("CUI_SkillSlot_Skill03"), Skill03Slot);
    if (UltimateSkillSlot) SkillSlotLookup.Add(TEXT("CUI_SkillSlot_UltimateSkill"), UltimateSkillSlot);
}

void UGOSkillSetBarWidget::InitializeSkillSlots(UGOSkills* SkillSet)
{
    if (!SkillSet)
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI InitializeSkillSlots] SkillSet is not null"));

    // Ω∫≈≥¿ª ΩΩ∑‘ ¿ß¡¨ø° ∏≈«Œ
    const TMap<ECharacterSkills, UGOSkillBase*>& Skills = SkillSet->GetSkills();
    for (const auto& Skill : Skills)
    {
        UGOSkillSlotWidget** SlotWidget = SkillSlotLookup.Find(SkillKeyToName(Skill.Key));
        if (SlotWidget && *SlotWidget)
        {
            (*SlotWidget)->BindSkill(Skill.Value);

        }
    }
}

FName UGOSkillSetBarWidget::SkillKeyToName(ECharacterSkills SkillType)
{
    switch (SkillType)
    {
    case ECharacterSkills::BaseSkill: return FName("CUI_SkillSlot_BaseSkill");
    case ECharacterSkills::Skill01: return FName("CUI_SkillSlot_Skill01");
    case ECharacterSkills::Skill02: return FName("CUI_SkillSlot_Skill02");
    case ECharacterSkills::Skill03: return FName("CUI_SkillSlot_Skill03");
    case ECharacterSkills::UltimateSkill: return FName("CUI_SkillSlot_UltimateSkill");
    default: return FName();
    }
}