// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GOGrindingStoneWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnYesButtonClickedDelegate);

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOGrindingStoneWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	// NativeConstruct override
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* YesButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UCommonTextBlock* PercentText;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* StrengthText;

	UPROPERTY(meta = (BindWidget))
	class UImage* GrindingStoneImage;

	UFUNCTION()
	void HandleYesButtonClicked();

	UFUNCTION()
	void HandleCancelButtonClicked();

	FOnYesButtonClickedDelegate OnYesButtonClicked;

	UFUNCTION(BlueprintCallable, Category = "GrindingStone")
	void ShowResultMessage(bool bSuccess, int32 StatIncreaseAmount);
};
