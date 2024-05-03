// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "UI/GOSkillSlotWidget.h"
#include "Skill/Slots/GOSkills.h"
#include "GOSkillSetBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOSkillSetBarWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	TMap<FName, UGOSkillSlotWidget*> SkillSlotLookup;
	
	// 스킬 슬롯 위젯 클래스 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGOSkillSlotWidget> SkillSlotWidgetClass;

	void InitializeSkillSlots(UGOSkills* SkillSet);
	FName SkillKeyToName(ECharacterSkills SkillType);

};
