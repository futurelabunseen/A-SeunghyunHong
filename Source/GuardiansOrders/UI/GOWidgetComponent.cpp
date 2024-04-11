// Fill out your copyright notice in the Description page of Project Settings.


#include "GOWidgetComponent.h"
#include "GOUserWidget.h"

UGOWidgetComponent::UGOWidgetComponent()
{

}

void UGOWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UGOUserWidget* GOUserWidget = Cast<UGOUserWidget>(GetWidget());
	if (GOUserWidget)
	{
		GOUserWidget->SetOwningActor(GetOwner());
	}
}