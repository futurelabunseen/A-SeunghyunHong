// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOSkillSlotWidget.h"
#include "CommonTextBlock.h"
#include "Skill/GOSkillBase.h"
#include <Kismet/GameplayStatics.h>

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

void UGOSkillSlotWidget::NativeTick(const FGeometry& Geometry, float DeltaSeconds)
{
    Super::NativeTick(Geometry, DeltaSeconds);

    //if (!bIsCooldownActive || !CurrentSkill)
    //    return;  // 쿨다운이 활성화되어 있지 않거나 스킬 정보가 없으면 리턴
    //CooldownStartTime = UGameplayStatics::GetTimeSeconds(GetWorld());

    //float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());  // 현재 게임 시간
    //float TimeElapsed = CurrentTime - CooldownStartTime;
    //float CooldownDuration = CurrentSkill->GetCoolDownTime();
    //float RemainingTime = CooldownDuration - TimeElapsed;

    //UpdateCooldownUI(RemainingTime);

    //if (RemainingTime <= 0.f)
    //{
    //    bIsCooldownActive = false;  // 쿨다운 종료
    //    CooldownImage->SetVisibility(ESlateVisibility::Hidden);
    //    CooldownText->SetVisibility(ESlateVisibility::Hidden);
    //    CooldownStartTime = 0.f;
    //    SetCooldownActive(false);
    //}



    if (bIsCooldownActive && CurrentSkill)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ UGOSkillSlotWidget::NativeTick] CurrentSkill %s"), *CurrentSkill.GetName());

        float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
        float TimeElapsed = CurrentTime - CooldownStartTime;
        float CooldownDuration = CurrentSkill->GetCoolDownTime();
        float RemainingTime = CooldownDuration - TimeElapsed;

        if (RemainingTime <= 0)
        {
            bIsCooldownActive = false;
            CurrentSkill->SetIsOnCoolTime(false);
            OnCooldownChanged(false);
        }
        else
        {
            // 쿨다운 시간 업데이트
            FString FormattedTime = FString::Printf(TEXT("%.1f"), RemainingTime);
            CooldownText->SetText(FText::FromString(FormattedTime));

            if (MatInstance)
            {
                float Percent = 1.0f - (RemainingTime / CooldownDuration);
                MatInstance->SetScalarParameterValue(FName("Percent"), Percent);
            }
        }
    }
}

void UGOSkillSlotWidget::BindSkill(UGOSkillBase* Skill)
{
    if (!Skill)
    {
        return;
    }
    CurrentSkill = Skill;
        
    //CurrentSkill->OnCooldownUpdated.AddUObject(this, &UGOSkillSlotWidget::UpdateCooldownUI);
    CurrentSkill->UGOSkillBaseFIsOnCooldown.AddUObject(this, &UGOSkillSlotWidget::OnCooldownChanged);
    SkillIconImage->SetBrushFromTexture(Skill->GetTotalSkillData().SkillIcon); // 스킬 아이콘 설정
    
    UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI BindSkill] SkillIconImage is %s"), *CurrentSkill->GetTotalSkillData().SkillIcon.GetName());
    UE_LOG(LogTemp, Warning, TEXT("Skill bound and delegate bound for cooldown updates."));

    // bIsCooldownActive = true;  // 쿨다운 시작

}

void UGOSkillSlotWidget::UpdateCooldownUI(float DeltaTime /*CooldownRemaining*/)
{
    UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UpdateCooldownUI] is called. %f"), DeltaTime);
    //if (testVal < DeltaTime)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI ???]")); // 버그
    //}

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

void UGOSkillSlotWidget::OnCooldownChanged(bool bIsActive)
{
    bIsCooldownActive = bIsActive;
    UE_LOG(LogTemp, Warning, TEXT("[UGOSkillSlotWidget::OnCooldownChanged] is called. %d"), bIsCooldownActive);

    if (bIsActive)
    {
        CooldownStartTime = UGameplayStatics::GetTimeSeconds(GetWorld());
        CooldownImage->SetVisibility(ESlateVisibility::Visible);
        CooldownText->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        CooldownImage->SetVisibility(ESlateVisibility::Hidden);
        CooldownText->SetVisibility(ESlateVisibility::Hidden);
    }
}
