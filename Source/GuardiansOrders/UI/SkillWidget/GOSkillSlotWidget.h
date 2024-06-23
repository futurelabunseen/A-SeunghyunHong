// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"

#include "GOSkillSlotWidget.generated.h"

class UCommonTextBlock;
class UGOSkillBase;
class UGOCharacterStatComponent;
class UGOSkillSlotToolTipWidget;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOSkillSlotWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& Geometry, float DeltaSeconds) override;
    UPROPERTY(meta = (BindWidget))
    UImage* SkillIconImage;

    UPROPERTY(meta = (BindWidget))
    UImage* CooldownImage;

    UPROPERTY(meta = (BindWidget))
    UImage* NoManaImage;

    UPROPERTY(meta = (BindWidget))
    UCommonTextBlock* CooldownText;

    UMaterialInstanceDynamic* MatInstance;

    void BindSkill(UGOSkillBase* Skill);

    void UpdateCooldownUI(float DeltaTime);

    TObjectPtr<UGOSkillBase> CurrentSkill;

    bool bIsCooldownActive = false;  // 쿨다운이 활성화되어 있는지 확인
    float CooldownStartTime = 0.f;  // 쿨다운 시작 시간

    FORCEINLINE void SetCooldownActive(bool InBool) { bIsCooldownActive = InBool; }
    void OnCooldownChanged(bool bIsActive);

public:
    void BindCharacterComponent(UGOCharacterStatComponent* CharacterStatComp);
    void HandleManaChange(float CurrentMana);

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGOSkillSlotToolTipWidget> ToolTipClass;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UGOSkillSlotToolTipWidget> GOToolTipWidget;
}; 
