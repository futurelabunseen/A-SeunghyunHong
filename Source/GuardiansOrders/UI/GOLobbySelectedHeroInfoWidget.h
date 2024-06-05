// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "Share/ShareEnums.h"
#include "GameData/GOCharacterData.h"
#include <GameData/GOGameSubsystem.h>
#include "GOLobbySelectedHeroInfoWidget.generated.h"
class UCommonTextBlock;
class UGOCharacterStatWidget;
class UGOSkillSlotLobbyWidget;
class UGOSkillSetBarLobbyWidget;
/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOLobbySelectedHeroInfoWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UGOLobbySelectedHeroInfoWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	UImage* HeroImage;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* RoleTypeText;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* AttackRangeText;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* ArchetypeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGOCharacterStatWidget> HeroStatWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGOSkillSetBarLobbyWidget> HeroSkillSetBarWidget;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnHeroSelected(EHeroType HeroType);

	void UpdateHeroInfo(EHeroType HeroType);

	void SetHeroImage(UTexture2D* Image);
	void SetNameText(const FText& Name);
	void SetRoleTypeText(const FText& Role);
	void SetAttackRangeText(const FText& Range);
	void SetArchetypeText(const FText& Archetype);

	FString GetEnumValueAsString(const FString& EnumName, int32 EnumValue);
	void BindToPlayerState();

private:
	void UpdateSkillIcons(FGOCharacterData* CharacterData, UGOGameSubsystem* GameSubsystem);
	void SetSkillIcon(UGOSkillSlotLobbyWidget* SkillSlotWidget, FName SkillName, UGOGameSubsystem* GameSubsystem);
};
