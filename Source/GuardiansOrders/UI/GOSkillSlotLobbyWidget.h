// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "GOSkillSlotLobbyWidget.generated.h"

class UGOSkillSlotToolTipWidget;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOSkillSlotLobbyWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UImage* SkillIconImage;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGOSkillSlotToolTipWidget> ToolTipClass;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UGOSkillSlotToolTipWidget> GOToolTipWidget;

};
