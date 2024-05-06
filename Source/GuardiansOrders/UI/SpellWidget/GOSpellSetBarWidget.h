// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "UI/SpellWidget/GOSpellSlotWidget.h"
#include "Skill/Slots/GOSpells.h"
#include "GOSpellSetBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOSpellSetBarWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spells")
	TMap<FName, UGOSpellSlotWidget*> SpellSlotLookup;

	// 스킬 슬롯 위젯 클래스 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGOSpellSlotWidget> SpellSlotWidgetClass;

	void InitializeSpellSlots(UGOSpells* SkillSet);
	FName SpellKeyToName(ECharacterSpells SpellType);
};
