// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOCharacterStatWidget.h"
#include "CommonTextBlock.h"

void UGOCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (TFieldIterator<FNumericProperty> PropIt(FGOCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		//const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		UCommonTextBlock* BaseTextBlock = Cast<UCommonTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		//UCommonTextBlock* ModifierTextBlock = Cast<UCommonTextBlock>(GetWidgetFromName(TextModifierControlName));
		//if (ModifierTextBlock)
		//{
		//	ModifierLookup.Add(PropKey, ModifierTextBlock);
		//}
	}
}

void UGOCharacterStatWidget::UpdateStat(const FGOCharacterStat& BaseStat, const FGOCharacterStat& ModifierStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FGOCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);
		/*float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);*/

		UCommonTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		/*UCommonTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}*/
	}
}
