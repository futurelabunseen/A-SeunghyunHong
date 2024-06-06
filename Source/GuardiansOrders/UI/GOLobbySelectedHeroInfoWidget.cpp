#include "UI/GOLobbySelectedHeroInfoWidget.h"
#include "Player/GOLobbyPlayerController.h"
#include "Game/GOPlayerState.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "CommonTextBlock.h"
#include <GameData/GOGameSubsystem.h>
#include "GameData/GOCharacterData.h"
#include "UI/GOCharacterStatWidget.h"
#include "UI/GOSkillSlotLobbyWidget.h"
#include "UI/GOSkillSlotToolTipWidget.h"
#include "UI/GOSkillSetBarLobbyWidget.h"
#include "UI/GOSkillSlotToolTipWidget.h"

UGOLobbySelectedHeroInfoWidget::UGOLobbySelectedHeroInfoWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UGOLobbySelectedHeroInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();
    BindToPlayerState();

    OnHeroSelected(EHeroType::Rogers);
}

void UGOLobbySelectedHeroInfoWidget::BindToPlayerState()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        AGOLobbyPlayerController* LobbyPlayerController = Cast<AGOLobbyPlayerController>(PlayerController);
        if (LobbyPlayerController)
        {
            AGOPlayerState* PS = LobbyPlayerController->GetPlayerState<AGOPlayerState>();
            if (PS)
            {
                // Initial bind to current selected hero if already set
                OnHeroSelected(PS->SelectedHeroInfo);

            }
        }
    }
}

void UGOLobbySelectedHeroInfoWidget::UpdateSkillIcons(FGOCharacterData* CharacterData, UGOGameSubsystem* GameSubsystem)
{
    if (CharacterData && HeroSkillSetBarWidget && GameSubsystem)
    {
        // Fetch and set the skill icons
        SetSkillIcon(HeroSkillSetBarWidget->SkillSlot_LMB_Widget, CharacterData->DefaultBaseSkillName, GameSubsystem);
        SetSkillIcon(HeroSkillSetBarWidget->SkillSlot_Q_Widget, CharacterData->DefaultSkillNameQ, GameSubsystem);
        SetSkillIcon(HeroSkillSetBarWidget->SkillSlot_W_Widget, CharacterData->DefaultSkillNameW, GameSubsystem);
        SetSkillIcon(HeroSkillSetBarWidget->SkillSlot_E_Widget, CharacterData->DefaultSkillNameE, GameSubsystem);
        SetSkillIcon(HeroSkillSetBarWidget->SkillSlot_R_Widget, CharacterData->DefaultSkillNameR, GameSubsystem);
    }
}

void UGOLobbySelectedHeroInfoWidget::OnHeroSelected(EHeroType HeroType)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        AGOLobbyPlayerController* LobbyPlayerController = Cast<AGOLobbyPlayerController>(PlayerController);
        if (LobbyPlayerController)
        {
            AGOPlayerState* PS = LobbyPlayerController->GetPlayerState<AGOPlayerState>();
            if (PS)
            {
                UE_LOG(LogTemp, Warning, TEXT("[UGOLobbySelectedHeroInfoWidget] OnHeroSelected 11 %d "), PS->SelectedHeroInfo);
            }
        }
    }
    UpdateHeroInfo(HeroType);
}

void UGOLobbySelectedHeroInfoWidget::UpdateHeroInfo(EHeroType HeroType)
{
    UGOGameSubsystem* GameSubsystem = GetGameInstance()->GetSubsystem<UGOGameSubsystem>();
    if (GameSubsystem)
    {
        FName HeroName = GameSubsystem->GetHeroTypeFName(HeroType);
        FGOCharacterData* CharacterData = GameSubsystem->GetCharacterData(HeroName);
        if (CharacterData)
        {
            SetHeroImage(CharacterData->HeroLobbyInfoImage);
            SetNameText(FText::FromName(HeroName));
            SetRoleTypeText(FText::FromString(GetEnumValueAsString("ERoleType", static_cast<int32>(CharacterData->RoleType))));
            SetAttackRangeText(FText::FromString(GetEnumValueAsString("EAttackRange", static_cast<int32>(CharacterData->AttackRangeType))));
            SetArchetypeText(FText::FromString(GetEnumValueAsString("EArchetype", static_cast<int32>(CharacterData->ArcheType))));
        
            FGOCharacterStat* CharacterStat = GameSubsystem->GetCharacterStatData(HeroName);
            if (CharacterStat && HeroStatWidget)
            {
                HeroStatWidget->UpdateStat(*CharacterStat, *CharacterStat); // Assuming both base and modifier stats are the same for simplicity.
            }
            UpdateSkillIcons(CharacterData, GameSubsystem);
        }
    }
}

void UGOLobbySelectedHeroInfoWidget::SetHeroImage(UTexture2D* Image)
{
    if (HeroImage && Image)
    {
        HeroImage->SetBrushFromTexture(Image);
    }
}

void UGOLobbySelectedHeroInfoWidget::SetNameText(const FText& Name)
{
    if (NameText)
    {
        NameText->SetText(Name);
    }
}

void UGOLobbySelectedHeroInfoWidget::SetRoleTypeText(const FText& Role)
{
    if (RoleTypeText)
    {
        RoleTypeText->SetText(Role);
    }
}

void UGOLobbySelectedHeroInfoWidget::SetAttackRangeText(const FText& Range)
{
    if (AttackRangeText)
    {
        AttackRangeText->SetText(Range);
    }
}

void UGOLobbySelectedHeroInfoWidget::SetArchetypeText(const FText& Archetype)
{
    if (ArchetypeText)
    {
        ArchetypeText->SetText(Archetype);
    }
}

FString UGOLobbySelectedHeroInfoWidget::GetEnumValueAsString(const FString& EnumName, int32 EnumValue)
{
    const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
    if (!EnumPtr) return FString("Invalid");

    return EnumPtr->GetNameStringByIndex(EnumValue);
}

void UGOLobbySelectedHeroInfoWidget::SetSkillIcon(UGOSkillSlotLobbyWidget* SkillSlotWidget, FName SkillName, UGOGameSubsystem* GameSubsystem)
{
    UE_LOG(LogTemp, Warning, TEXT("[SetSkillIcon] 0"));
    UGOGameSubsystem* GOGameSubsystem = GetGameInstance()->GetSubsystem<UGOGameSubsystem>();
    if (GOGameSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("[SetSkillIcon] 0.5"));

        if (SkillSlotWidget && GOGameSubsystem)
        {
            UE_LOG(LogTemp, Warning, TEXT("[SetSkillIcon] 1"));

            FGOSkillData* SkillData = GOGameSubsystem->GetSkillData(SkillName);
            if (SkillData && SkillData->SkillIcon)
            {
                UE_LOG(LogTemp, Warning, TEXT("[SetSkillIcon] 2"));

                SkillSlotWidget->SkillIconImage->SetBrushFromTexture(SkillData->SkillIcon);

                if (SkillSlotWidget->GOToolTipWidget)
                {
                    UGOSkillSlotToolTipWidget* GOToolTipWidget = Cast<UGOSkillSlotToolTipWidget>(SkillSlotWidget->GOToolTipWidget);
                    if (GOToolTipWidget)
                    {
                        GOToolTipWidget->UpdateSkillToolTip(SkillName);
                    }
                }
            }
        }
    }
}