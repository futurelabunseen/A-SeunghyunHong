// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SpellWidget/GOSpellSlotWidget.h"
#include "CommonTextBlock.h"
#include "Skill/GOSpellBase.h"
#include "CharacterStat/GOCharacterStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Share/EditorNames.h"

void UGOSpellSlotWidget::NativeConstruct()
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

void UGOSpellSlotWidget::NativeTick(const FGeometry& Geometry, float DeltaSeconds)
{
    Super::NativeTick(Geometry, DeltaSeconds);

    if (bIsCooldownActive && CurrentSpell)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ UGOSkillSlotWidget::NativeTick] CurrentSkill %s"), *CurrentSpell.GetName());

        float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
        float TimeElapsed = CurrentTime - CooldownStartTime;
        float CooldownDuration = CurrentSpell->GetCoolDownTime();
        float RemainingTime = CooldownDuration - TimeElapsed;

        if (RemainingTime <= 0)
        {
            bIsCooldownActive = false;
            CurrentSpell->SetIsOnCoolTime(false);
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
                //MatInstance->SetScalarParameterValue(FName("Percent"), Percent);
                MatInstance->SetScalarParameterValue(GOMaterial::PERCENT, Percent);
            }
        }
    }
}

void UGOSpellSlotWidget::BindSpell(UGOSpellBase* Spell)
{
    UE_LOG(LogTemp, Warning, TEXT("[UGOSpellSlotWidget::BindSpell] --- Spell is  %s"), *Spell->GetName());

    if (!Spell)
    {
        return;
    }
    CurrentSpell = Spell;
    UE_LOG(LogTemp, Warning, TEXT("[UGOSpellSlotWidget::BindSpell] 000 Spell is  %s"), *Spell->GetName());

    //CurrentSkill->OnCooldownUpdated.AddUObject(this, &UGOSkillSlotWidget::UpdateCooldownUI);
    CurrentSpell->UGOSpellBaseFIsOnCooldown.AddUObject(this, &UGOSpellSlotWidget::OnCooldownChanged);
    SpellIconImage->SetBrushFromTexture(Spell->GetTotalSpellData().SpellIcon); // 스킬 아이콘 설정
    UE_LOG(LogTemp, Warning, TEXT("[UGOSpellSlotWidget::BindSpell] 111 Spell is  %s"), *Spell->GetName());
}

void UGOSpellSlotWidget::UpdateCooldownUI(float DeltaTime)
{
    CooldownImage->SetVisibility(ESlateVisibility::Visible);
    CooldownText->SetVisibility(ESlateVisibility::Visible);

    if (CooldownText)
    {
        FString FormattedTime = FString::Printf(TEXT("%.1f"), DeltaTime);
        CooldownText->SetText(FText::FromString(FormattedTime));
    }
    if (MatInstance)
    {
        float Percent = (CurrentSpell->GetCoolDownTime() - DeltaTime) / CurrentSpell->GetCoolDownTime();
        //MatInstance->SetScalarParameterValue(FName("percent"), Percent);
        MatInstance->SetScalarParameterValue(GOMaterial::PERCENT, Percent);
    }
}

void UGOSpellSlotWidget::OnCooldownChanged(bool bIsActive)
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
