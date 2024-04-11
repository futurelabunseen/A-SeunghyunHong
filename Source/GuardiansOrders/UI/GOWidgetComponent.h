// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GOWidgetComponent.generated.h"

/**
 *
 */
UCLASS()
class GUARDIANSORDERS_API UGOWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UGOWidgetComponent();

	/**
	�� �Լ��� ȣ��� ���� ������ ���� �ν��Ͻ��� ������ �����Դϴ�.
	*/
	virtual void InitWidget() override;
};
