// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "GOSpellSlotWidget.generated.h"

class UCommonTextBlock;
class UGOSpellBase;
class UGOCharacterStatComponent;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOSpellSlotWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& Geometry, float DeltaSeconds) override;
    UPROPERTY(meta = (BindWidget))
    UImage* SpellIconImage;

    UPROPERTY(meta = (BindWidget))
    UImage* CooldownImage;

    UPROPERTY(meta = (BindWidget))
    UCommonTextBlock* CooldownText;

    UMaterialInstanceDynamic* MatInstance;

    void BindSpell(UGOSpellBase* Spell);

    void UpdateCooldownUI(float DeltaTime);

    TObjectPtr<UGOSpellBase> CurrentSpell;

    bool bIsCooldownActive = false;  // ��ٿ��� Ȱ��ȭ�Ǿ� �ִ��� Ȯ��
    float CooldownStartTime = 0.f;  // ��ٿ� ���� �ð�

    FORCEINLINE void SetCooldownActive(bool InBool) { bIsCooldownActive = InBool; }
    void OnCooldownChanged(bool bIsActive);
};
