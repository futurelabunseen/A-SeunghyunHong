// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Share/ShareEnums.h"
#include "GOLobbyTeamMemberWidget.generated.h"

class UImage;
class UCommonTextBlock;
class UOverlay;
/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOLobbyTeamMemberWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UGOLobbyTeamMemberWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	void SetTeamMemberUI(FHeroSelectionInfo HeroInfo);

	UPROPERTY(meta = (BindWidget))
	UImage* HeroIconImage;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* HeroNameText;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* SteamNameText;
	
	UPROPERTY(meta = (BindWidget))
	UOverlay* RingOverlay; // NS_Ring Widget

	FText GetEHeroTypeAsText(EHeroType HeroType)
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
