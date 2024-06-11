// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Share/ShareEnums.h"
#include "GOHeroSelectionWidget.generated.h"

//UFUNCTION(Server, Reliable, WithValidation)
//void ServerSelectHero(APlayerController* PlayerController, EHeroType HeroType);
class UButton;
class UEditableTextBox;
class UHorizontalBox;

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

    UFUNCTION()
    void OnReadyButtonClicked(); // Ready ��ư Ŭ�� �̺�Ʈ �ڵ鷯

    UFUNCTION()
    void EnableSetNicknameSection();

    UFUNCTION()
    void EnableReadyButton();    
    
    
    UFUNCTION()
    void OnNicknameButtonClicked();

    // UMG widget bindings
    UPROPERTY(meta = (BindWidget))
    UButton* RogersButton;

    UPROPERTY(meta = (BindWidget))
    UButton* KatnissButton;

    UPROPERTY(meta = (BindWidget))
    UButton* BeastButton;

    UPROPERTY(meta = (BindWidget))
    UButton* BrideButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ReadyButton;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* EditableNicknameText;

    UPROPERTY(meta = (BindWidget))
    UButton* NicknameButton;


    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* SetNicknameBox;
private:
    // �߰�
    // void SelectCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass);
    void SelectCharacter(EHeroType HeroType);

};
