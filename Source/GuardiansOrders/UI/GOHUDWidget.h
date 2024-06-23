// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameData/GOCharacterStat.h"
#include "Containers/Queue.h"
#include "GOHUDWidget.generated.h"

class UGOStatsBarWidget;
class UGOHpBarWidget;
class UGOManaBarWidget;
class UGOCharacterStatWidget;
class UGOSkillSetBarWidget;
class UGOSpellSetBarWidget;
class UGOHeroInfoWidget;
class UCommonUserWidget;
class UGOGrindingStoneWidget;
class UCommonTextBlock;
class UGOReturnToMainWidget;

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

	void AddElimAnnouncement(FString Attacker, FString Victim);

	UPROPERTY()
	class UGOBattleCharacterOverlayWidget* CharacterOverlay;
	
	UPROPERTY(meta = (BindWidget))
	UGOGrindingStoneWidget* GrindingStoneWidget;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* WinnerText;

	UPROPERTY(meta = (BindWidget))
	UGOReturnToMainWidget* WidgetReturnToMainMenu;

	UPROPERTY(meta = (BindWidget))
	class UImage* ReturnToMainBGImage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGOElimAnnouncementWidget> GOElimAnnouncementClass;

	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 3.f;

	UFUNCTION()
	void ElimAnnouncementTimerFinished(UGOElimAnnouncementWidget* MsgToRemove);
	
	UPROPERTY()
	TArray<TObjectPtr<UGOElimAnnouncementWidget>> ElimMessages;

	UFUNCTION()
	void AnnouncementStatIncrease(float OriginStat, float IncreaseAmount, float NewStat);
	// 모델 델리게이트를 바인딩하는 함수
	void BindToStatComponent(class UGOCharacterStatComponent* StatComponent);

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* StatAnnouncementText;

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
