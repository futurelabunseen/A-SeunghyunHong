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
		// UEnum ã�� (������ UENUM Ŭ����)
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EHeroType"), true);
		if (!EnumPtr) return FText::GetEmpty();

		// Enum ���� ���ڿ��� ��������
		FString HeroTypeName = EnumPtr->GetNameStringByValue((int64)HeroType);

		// ���ڿ��� �빮�ڷ� ��ȯ
		HeroTypeName = HeroTypeName.ToUpper();

		// FString�� FText�� ��ȯ
		return FText::FromString(HeroTypeName);
	}
};
