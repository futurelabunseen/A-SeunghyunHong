// Fill out your copyright notice in the Description page of Project Settings.


#include "GOPlayerController.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "UI/GOHUDWidget.h"
#include "CommonActivatableWidget.h"
#include "UI/SkillWidget/GOSkillSetBarWidget.h"
#include <Game/GOPlayerState.h>
#include <GameData/GOGameSubsystem.h>
#include <Character/GOPlayerCharacter.h>
#include "UI/GOBattleCharacterOverlayWidget.h"
#include "CommonTextBlock.h"

AGOPlayerController::AGOPlayerController()
{
    /*
    static ConstructorHelpers::FClassFinder<UGOHUDWidget> GOHUDWidgetRef(TEXT("/Game/UI/HUD/CWBP_GOHUD.CWBP_GOHUD_C"));
    if (GOHUDWidgetRef.Class)
    {
        GOHUDWidgetClass = GOHUDWidgetRef.Class;
    }
    */
}

void AGOPlayerController::PostInitializeComponents()
{
    GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

    Super::PostInitializeComponents();

    GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOPlayerController::PostNetInit()
{
    GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

    Super::PostNetInit();

    UNetDriver* NetDriver = GetNetDriver();
    if (NetDriver)
    {
        if (NetDriver->ServerConnection)
        {
            GO_LOG(LogGONetwork, Log, TEXT("Server Connection: %s"), *NetDriver->ServerConnection->GetName());
        }
    }
    else
    {
        GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
    }
    GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOPlayerController::PostSeamlessTravel()
{
    Super::PostSeamlessTravel();

    AGOPlayerState* PS = GetPlayerState<AGOPlayerState>();
    if (PS)
    {
        int32 PlayerId = PS->GetPlayerId();
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
                FString::Printf(TEXT("PostSeamlessTravel - PlayerId: %d"), PlayerId));
        }
    }

    //SpawnAndPossessCharacter();

}

void AGOPlayerController::BeginPlay()
{
    GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Start"));
    Super::BeginPlay();
    GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetHideCursorDuringCapture(false);
    SetInputMode(InputModeData);

    
    if (IsLocalPlayerController() && IsValid(GOHUDWidgetClass))
    {
        GOHUDWidget = CreateWidget<UGOHUDWidget>(this, GOHUDWidgetClass);

        if (GOHUDWidget)
        {
            GOHUDWidget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay -2"));

            if (GOHUDWidget->CharacterOverlayClass)
            {
                UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay -1"));

                GOHUDWidget->AddCharacterOverlay();

            }

        }

    }
}

void AGOPlayerController::OnPossess(APawn* InPawn)
{
    GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

    Super::OnPossess(InPawn);

    GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOPlayerController::SetHUDScore(float Score)
{
    if (GOHUDWidget)
    {
        bool bHUDVaild = GOHUDWidget &&
            GOHUDWidget->CharacterOverlay &&
            GOHUDWidget->CharacterOverlay->ScoreAmount;
    
        if (bHUDVaild)
        {
            FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
            GOHUDWidget->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
        }
    }
}

void AGOPlayerController::InitializeSkills()
{

}

void AGOPlayerController::SpawnAndPossessCharacter()
{
    EHeroType SelectedHero = GetSelectedHero();
    if (SelectedHero == EHeroType::None)
    {
        UE_LOG(LogTemp, Warning, TEXT("No hero selected."));
        return;
    }

    UGOGameSubsystem* GameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGOGameSubsystem>();
    if (!GameSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameSubsystem not found."));
        return;
    }

    TSubclassOf<AGOPlayerCharacter> CharacterClass = GameSubsystem->GetCharacterClassByHeroType(SelectedHero);
    if (!CharacterClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character class not found for hero type %d"), static_cast<int8>(SelectedHero));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetPawn();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FVector NewSpawnLocation = FVector(0.0f, 0.0f, 100.0f);
    FRotator NewSpawnRotation = FRotator::ZeroRotator;

    AGOPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AGOPlayerCharacter>(CharacterClass, NewSpawnLocation, NewSpawnRotation, SpawnParams);
    if (NewCharacter)
    {
        Possess(NewCharacter);
        UE_LOG(LogTemp, Warning, TEXT("[POSSESS] possessed."));

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn pawn."));
    }
}

EHeroType AGOPlayerController::GetSelectedHero()
{
    AGOPlayerState* PS = GetPlayerState<AGOPlayerState>();
    if (PS)
    {
        return PS->SelectedHero.SelectedHero;
    }
    return EHeroType::None;
}

