// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOSkillSlotWidget.h"
#include "CommonTextBlock.h"
#include "Skill/GOSkillBase.h"

void UGOSkillSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CooldownImage)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CoolDown] CoolDownImage is not null"));

        UObject* ResourceObject = CooldownImage->GetBrush().GetResourceObject();
        if (ResourceObject)
        {
            UE_LOG(LogTemp, Warning, TEXT("[CoolDown] ResourceObject is %s"), *ResourceObject->GetName()); // M_ClockScan

            UMaterialInterface* BaseMaterial = Cast<UMaterialInterface>(ResourceObject);
            if (BaseMaterial)
            {
                MatInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this); // 내가 설정한 매터리얼에 대한 동적 인스턴스를 생성
                if (MatInstance)
                {
                    CooldownImage->SetBrushFromMaterial(MatInstance);
                    CooldownImage->SetVisibility(ESlateVisibility::Hidden);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to create Material Instance Dynamic from BaseMaterial."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to cast ResourceObject to UMaterialInterface."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ResourceObject is nullptr."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CooldownImage is nullptr."));
    }

    if (CooldownText)
    {
        CooldownText->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UGOSkillSlotWidget::BindSkill(UGOSkillBase* Skill)
{
    if (!Skill)
    {
        return;
    }
    CurrentSkill = Skill;
        
    CurrentSkill->OnCooldownUpdated.AddUObject(this, &UGOSkillSlotWidget::UpdateCooldownUI);

    SkillIconImage->SetBrushFromTexture(Skill->GetTotalSkillData().SkillIcon); // 스킬 아이콘 설정
    
    UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI BindSkill] SkillIconImage is %s"), *CurrentSkill->GetTotalSkillData().SkillIcon.GetName());
    UE_LOG(LogTemp, Warning, TEXT("Skill bound and delegate bound for cooldown updates."));

}

void UGOSkillSlotWidget::UpdateCooldownUI(float DeltaTime /*CooldownRemaining*/)
{
    // UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UpdateCooldownUI] is called. %f"), DeltaTime);
    if (testVal < DeltaTime)
    {
        UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI ???]"));
    }

    CooldownImage->SetVisibility(ESlateVisibility::Visible);
    CooldownText->SetVisibility(ESlateVisibility::Visible);

    if (CooldownText)
    {
        FString FormattedTime = FString::Printf(TEXT("%.1f"), DeltaTime);
        CooldownText->SetText(FText::FromString(FormattedTime));
    }
    if (MatInstance)
    {
        float Percent = (CurrentSkill->GetCoolDownTime() - DeltaTime) / CurrentSkill->GetCoolDownTime();
        MatInstance->SetScalarParameterValue(FName("percent"), Percent);
    }
}
