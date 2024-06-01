// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameData/GOCharacterStat.h"
#include "GOHUDWidget.generated.h"

class UGOStatsBarWidget;
class UGOHpBarWidget;
class UGOManaBarWidget;
class UGOCharacterStatWidget;
class UGOSkillSetBarWidget;
class UGOSpellSetBarWidget;
class UGOHeroInfoWidget;

UCLASS()
class GUARDIANSORDERS_API UGOHUDWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UGOHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FGOCharacterStat& BaseStat, const FGOCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);
	void UpdateManaBar(float NewCurrentMana, float NewMaxMana);
	TObjectPtr<UGOSkillSetBarWidget> GetSkillSetBar() { return SkillSetBar; }
	TObjectPtr<UGOSpellSetBarWidget> GetSpellSetBar() { return SpellSetBar; }
	TObjectPtr<UGOHeroInfoWidget> GetHeroInfo() { return HeroInfo; }

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UGOBattleCharacterOverlayWidget> CharacterOverlayClass;

	void AddCharacterOverlay();

	UPROPERTY()
	class UGOBattleCharacterOverlayWidget* CharacterOverlay;

protected:
	UPROPERTY()
	TObjectPtr<UGOStatsBarWidget> StatsBar;

	UPROPERTY()
	TObjectPtr<UGOHpBarWidget> HpBar;	
	
	UPROPERTY()
	TObjectPtr<UGOManaBarWidget> ManaBar;

	UPROPERTY()
	TObjectPtr<UGOCharacterStatWidget> CharacterStat;

	UPROPERTY()
	TObjectPtr<UGOSkillSetBarWidget> SkillSetBar;

	UPROPERTY()
	TObjectPtr<UGOSpellSetBarWidget> SpellSetBar;

	UPROPERTY()
	TObjectPtr<UGOHeroInfoWidget> HeroInfo;
};
