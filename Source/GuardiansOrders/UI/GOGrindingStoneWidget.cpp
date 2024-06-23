// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOGrindingStoneWidget.h"
#include "Components/Button.h"

void UGOGrindingStoneWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (YesButton)
    {
        YesButton->OnClicked.AddDynamic(this, &UGOGrindingStoneWidget::HandleYesButtonClicked);
    }

    if (CancelButton)
    {
        CancelButton->OnClicked.AddDynamic(this, &UGOGrindingStoneWidget::HandleCancelButtonClicked);
    }
}

void UGOGrindingStoneWidget::HandleYesButtonClicked()
{
    // 연마석에 의한 로직 적용

    // 이펙트

    // 확률 저장
    OnYesButtonClicked.Broadcast();
    this->SetVisibility(ESlateVisibility::Hidden);
}

void UGOGrindingStoneWidget::HandleCancelButtonClicked()
{
    this->SetVisibility(ESlateVisibility::Hidden);
}

void UGOGrindingStoneWidget::ShowResultMessage(bool bSuccess, int32 StatIncreaseAmount)
{
    FString Message = bSuccess ?
        FString::Printf(TEXT("Stat increase successful! BaseDamage increased by %d"), StatIncreaseAmount) :
        FString::Printf(TEXT("Stat increase failed. %d"), StatIncreaseAmount);

    // 메시지를 표시하는 로직 추가 (예: 텍스트 블록에 설정하거나 팝업 창을 띄우는 등)
    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
}