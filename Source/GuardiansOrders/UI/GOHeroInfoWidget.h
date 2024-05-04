// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "GameData/GOCharacterData.h"
#include "GOHeroInfoWidget.generated.h"

class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOHeroInfoWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UImage* HeroIconImage;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* NameText;

	void BindHeroInfo(FGOCharacterData CharacterData);

	FText GetHeroTypeAsText(EHeroType HeroType)
	{
		// UEnum 찾기 (생성된 UENUM 클래스)
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EHeroType"), true);
		if (!EnumPtr) return FText::GetEmpty();

		// Enum 값을 문자열로 가져오기
		FString HeroTypeName = EnumPtr->GetNameStringByValue((int64)HeroType);

		// 문자열을 대문자로 변환
		HeroTypeName = HeroTypeName.ToUpper();

		// FString을 FText로 변환
		return FText::FromString(HeroTypeName);
	}
};
