// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOSkillSlotToolTipWidget.h"
#include "CommonTextBlock.h"
#include <GameData/GOGameSubsystem.h>

UGOSkillSlotToolTipWidget::UGOSkillSlotToolTipWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UGOSkillSlotToolTipWidget::UpdateSkillToolTip(FName SkillName)
{
    UGOGameSubsystem* GameSubsystem = GetGameInstance()->GetSubsystem<UGOGameSubsystem>();
    if (!GameSubsystem)
    {
        return;
    }

    FGOSkillData* SkillData = GameSubsystem->GetSkillData(SkillName);
    if (SkillData)
    {
        SkillNameText->SetText(FText::FromString(SkillData->SkillName));
        SkillDescriptionText->SetText(FText::FromString(SkillData->Description));
        SkillTriggerTypeText->SetText(FText::FromString(GetEnumValuesAsString("ESkillTriggerType", static_cast<int32>(SkillData->SkillTriggerType))));
        SkillAffectTypeText->SetText(FText::FromString(GetEnumValuesAsString("ESkillAffectType", static_cast<int32>(SkillData->SkillAffectType))));
        SkillCastTypeText->SetText(FText::FromString(GetEnumValuesAsString("ESkillCastType", static_cast<int32>(SkillData->SkillCastType))));

        FGOSkillStat* SkillStat = GameSubsystem->GetSkillStatData(SkillData->SkillStatName);
        if (SkillStat)
        {
            CoolTimeText->SetText(FText::AsNumber(SkillStat->CoolDownTime));
            ManaCostText->SetText(FText::AsNumber(SkillStat->ManaCost));
        }
    }
}


void UGOSkillSlotToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FString UGOSkillSlotToolTipWidget::GetEnumValuesAsString(const FString& EnumName, int32 EnumValue)
{
    const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
    if (!EnumPtr) return FString("Invalid");

    return EnumPtr->GetNameStringByIndex(EnumValue);
}