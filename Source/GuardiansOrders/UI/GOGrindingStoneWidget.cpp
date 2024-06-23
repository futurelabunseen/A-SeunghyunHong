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
    // �������� ���� ���� ����

    // ����Ʈ

    // Ȯ�� ����
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

    // �޽����� ǥ���ϴ� ���� �߰� (��: �ؽ�Ʈ ��Ͽ� �����ϰų� �˾� â�� ���� ��)
    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
}