// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SpellWidget/GOSpellSetBarWidget.h"
#include "Character/GOCharacterBase.h"

void UGOSpellSetBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UGOSpellSlotWidget* Spell01Slot = Cast<UGOSpellSlotWidget>(GetWidgetFromName(TEXT("CUI_SpellSlot_Spell01")));
    UGOSpellSlotWidget* Spell02Slot = Cast<UGOSpellSlotWidget>(GetWidgetFromName(TEXT("CUI_SpellSlot_Spell02")));

    if (Spell01Slot) SpellSlotLookup.Add(TEXT("CUI_SpellSlot_Spell01"), Spell01Slot);
    if (Spell02Slot) SpellSlotLookup.Add(TEXT("CUI_SpellSlot_Spell02"), Spell02Slot);
}

void UGOSpellSetBarWidget::InitializeSpellSlots(UGOSpells* SpellSet)
{
    if (!SpellSet)
    {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("[UGOSpellSetBarWidget::InitializeSpellSlots] --- SpellSet is  %s"), *SpellSet->GetName());

    // ��ų�� ���� ������ ����
    const TMap<ECharacterSpells, UGOSpellBase*>& Spells = SpellSet->GetSpells();
    for (const auto& Spell : Spells)
    {
        UGOSpellSlotWidget** SlotWidget = SpellSlotLookup.Find(SpellKeyToName(Spell.Key));

        AGOCharacterBase* OwnerCharacter = Cast<AGOCharacterBase>(GetOwningPlayerPawn()); // ������ �����ϰ� �ִ� ĳ���͸� ������
        if (!OwnerCharacter) return;
        UE_LOG(LogTemp, Warning, TEXT("[UGOSpellSetBarWidget::InitializeSpellSlots] 000"));

        // UGOCharacterStatComponent* CharacterStatComp = OwnerCharacter->GetStatComponent(); // ĳ������ ���� ������Ʈ�� ������

        //(*SlotWidget)->BindSpell(Spell.Value);
        if (SlotWidget && *SlotWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("[UGOSpellSetBarWidget::InitializeSpellSlots] 111"));

            (*SlotWidget)->BindSpell(Spell.Value);
            UE_LOG(LogTemp, Warning, TEXT("[UGOSpellSetBarWidget::InitializeSpellSlots] 222"));

        }
        
    }
}

FName UGOSpellSetBarWidget::SpellKeyToName(ECharacterSpells SpellType)
{
    switch (SpellType)
    {
    case ECharacterSpells::Spell01: return FName("CUI_SpellSlot_Spell01");
    case ECharacterSpells::Spell02: return FName("CUI_SpellSlot_Spell02");
    default: return FName();
    }
}
