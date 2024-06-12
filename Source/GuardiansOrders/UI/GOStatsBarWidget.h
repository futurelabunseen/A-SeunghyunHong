// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GOUserWidget.h"
#include "GOStatsBarWidget.generated.h"
class UProgressBar;
class UGOHpBarWidget;
class UGOManaBarWidget;
class UCommonTextBlock;
class UTextBlock;

UCLASS()
class GUARDIANSORDERS_API UGOStatsBarWidget : public UGOUserWidget
{
	GENERATED_BODY()
	
public:
	UGOStatsBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
    UPROPERTY()
    TObjectPtr < UGOHpBarWidget> HpBar;

    UPROPERTY()
    TObjectPtr < UGOManaBarWidget> ManaBar;

    //UPROPERTY(meta = (BindWidget))
    //UCommonTextBlock* NicknameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Nickname;
};
