// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOHUDWidget.h"
#include "Interface/GOCharacterHUDInterface.h"
#include "GOStatsBarWidget.h"
#include "GOHpBarWidget.h"
#include "GOManaBarWidget.h"
#include "GOCharacterStatWidget.h"
#include "SkillWidget/GOSkillSetBarWidget.h"
#include "SpellWidget/GOSpellSetBarWidget.h"
#include "GOHeroInfoWidget.h"
#include <Player/GOPlayerController.h>
#include "UI/GOBattleCharacterOverlayWidget.h"
#include "CommonUserWidget.h"
#include "UI/GOGrindingStoneWidget.h"
#include "UI/GOElimAnnouncementWidget.h"
#include "TimerManager.h" 
#include "Components/HorizontalBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "CommonTextBlock.h"
#include "CharacterStat/GOCharacterStatComponent.h"

UGOHUDWidget::UGOHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UGOHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StatsBar = Cast<UGOStatsBarWidget>(GetWidgetFromName(TEXT("WidgetStatsBar")));
	ensure(StatsBar);

	// Access the HpBar and ManaBar through the StatsBar
	HpBar = Cast<UGOHpBarWidget>(StatsBar->GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpBar);

	ManaBar = Cast<UGOManaBarWidget>(StatsBar->GetWidgetFromName(TEXT("PbManaBar")));
	ensure(ManaBar);

	//HpBar = Cast<UGOHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	//ensure(HpBar);

	//ManaBar = Cast<UGOManaBarWidget>(GetWidgetFromName(TEXT("WidgetManaBar")));
	//ensure(ManaBar);

	CharacterStat = Cast<UGOCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	SkillSetBar = Cast<UGOSkillSetBarWidget>(GetWidgetFromName(TEXT("CUI_SkillSetBar")));
	ensure(SkillSetBar);
	UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOHUDWidget] SkillSetBar: %s"), *SkillSetBar.GetName());

	SpellSetBar = Cast<UGOSpellSetBarWidget>(GetWidgetFromName(TEXT("CUI_SpellSetBar")));
	ensure(SpellSetBar);

	HeroInfo = Cast<UGOHeroInfoWidget>(GetWidgetFromName(TEXT("CUI_CharacterInfo")));

	GrindingStoneWidget = Cast<UGOGrindingStoneWidget>(GetWidgetFromName(TEXT("GrindingStoneWidget")));
	ensure(GrindingStoneWidget);
	UE_LOG(LogTemp, Warning, TEXT("GrindingStoneWidget name: %s"), *GrindingStoneWidget->GetName());

	IGOCharacterHUDInterface* HUDPawn = Cast<IGOCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}

	
}

void UGOHUDWidget::UpdateStat(const FGOCharacterStat& BaseStat, const FGOCharacterStat& ModifierStat)
{
	FGOCharacterStat TotalStat = BaseStat + ModifierStat;
	// HpBar->UpdateStat(BaseStat, ModifierStat);
	// ManaBar->UpdateStat(BaseStat, ModifierStat);
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UGOHUDWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	HpBar->UpdateHpBar(NewCurrentHp, NewMaxHp);
}

void UGOHUDWidget::UpdateManaBar(float NewCurrentMana, float NewMaxMana)
{
	ManaBar->UpdateManaBar(NewCurrentMana, NewMaxMana);
}

void UGOHUDWidget::AddCharacterOverlay()
{
	UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay 0"));

	APlayerController* BasePlayerController = GetWorld()->GetFirstPlayerController();
	AGOPlayerController* PlayerController = Cast<AGOPlayerController>(BasePlayerController);

	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay PlayerController is null"));
	}

	if (PlayerController && CharacterOverlayClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay 1"));

		CharacterOverlay = CreateWidget<UGOBattleCharacterOverlayWidget>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();		
		
		UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay 2"));
		
		//AddElimAnnouncement("Player1", "Player2");
	}
}

void UGOHUDWidget::AddElimAnnouncement(FString Attacker, FString Victim)
{
	APlayerController* BasePlayerController = GetWorld()->GetFirstPlayerController();
	//APlayerController* BasePlayerController = GetOwningPlayer();
	AGOPlayerController* OwningPlayer = Cast<AGOPlayerController>(BasePlayerController);
	if (OwningPlayer && GOElimAnnouncementClass)
	{
		UGOElimAnnouncementWidget* ElimAnnouncementWidget =
			CreateWidget<UGOElimAnnouncementWidget>(OwningPlayer, GOElimAnnouncementClass);

		if (ElimAnnouncementWidget)
		{
			ElimAnnouncementWidget->SetElimAnnouncementText(Attacker, Victim);
			ElimAnnouncementWidget->AddToViewport();
			
			for (UGOElimAnnouncementWidget* Msg : ElimMessages)
			{
				if (Msg && Msg->AnnouncementBox)
				{
					UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Msg->AnnouncementBox);
					if (CanvasSlot)
					{
						FVector2D Position = CanvasSlot->GetPosition();
						FVector2D NewPosition(
							CanvasSlot->GetPosition().X,
							Position.Y - CanvasSlot->GetSize().Y
						);
						CanvasSlot->SetPosition(NewPosition);		
						UE_LOG(LogTemp, Warning, TEXT("CanvasSlot SetPosition"));

					}
				}
			}

			ElimMessages.Add(ElimAnnouncementWidget);

			FTimerHandle ElimMsgTimer;
			FTimerDelegate ElimMsgDelegate;
			ElimMsgDelegate.BindUFunction(this, FName("ElimAnnouncementTimerFinished"), ElimAnnouncementWidget);
			OwningPlayer->GetWorldTimerManager().SetTimer(
				ElimMsgTimer,
				ElimMsgDelegate,
				ElimAnnouncementTime,
				false
			);
		}
	}
}

void UGOHUDWidget::ElimAnnouncementTimerFinished(UGOElimAnnouncementWidget* MsgToRemove)
{
	if (MsgToRemove)
	{
		MsgToRemove->RemoveFromParent();
	}
}

void UGOHUDWidget::AnnouncementStatIncrease(float OriginStat, float IncreaseAmount, float NewStat)
{
	FString AnnouncementText = FString::Printf(TEXT("STAT UP! %.0f + %.0f = %.0f"), OriginStat, IncreaseAmount, NewStat);
	StatAnnouncementText->SetText(FText::FromString(AnnouncementText));
}

void UGOHUDWidget::BindToStatComponent(UGOCharacterStatComponent* StatComponent)
{
	if (StatComponent)
	{
		StatComponent->OnStatIncreased.AddUObject(this, &UGOHUDWidget::AnnouncementStatIncrease);
	}
}
