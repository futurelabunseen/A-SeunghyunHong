// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "Skill/GOSkillBase.h"

#include "GOSkillSlotWidget.generated.h"

class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOSkillSlotWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(meta = (BindWidget))
    UImage* SkillIconImage;

    UPROPERTY(meta = (BindWidget))
    UImage* CooldownImage;

    UPROPERTY(meta = (BindWidget))
    UCommonTextBlock* CooldownText;

    void BindSkill(UGOSkillBase* Skill);
};
