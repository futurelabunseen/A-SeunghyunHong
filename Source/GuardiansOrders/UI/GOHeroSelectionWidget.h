// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GOHeroSelectionWidget.generated.h"
class UButton;
/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOHeroSelectionWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
    // NativeConstruct override
    virtual void NativeConstruct() override;

protected:
    // Button click event handlers
    UFUNCTION()
    void OnRogersButtonClicked();

    UFUNCTION()
    void OnBrideButtonClicked();

    UFUNCTION()
    void OnBeastButtonClicked();

    UFUNCTION()
    void OnKatnissButtonClicked();

    // UMG widget bindings
    UPROPERTY(meta = (BindWidget))
    UButton* RogersButton;

    UPROPERTY(meta = (BindWidget))
    UButton* KatnissButton;

    UPROPERTY(meta = (BindWidget))
    UButton* BeastButton;

    UPROPERTY(meta = (BindWidget))
    UButton* BrideButton;

private:
    // Ãß°¡
    void SelectCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass);
};
