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
	이 함수가 호출될 때는 위젯에 대한 인스턴스가 생성된 직후입니다.
	*/
	virtual void InitWidget() override;
};
