// Fill out your copyright notice in the Description page of Project Settings.


#include "GOPlayerController.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "UI/GOHUDWidget.h"
#include "CommonActivatableWidget.h"

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
        }
    }
    
}

void AGOPlayerController::OnPossess(APawn* InPawn)
{
    GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

    Super::OnPossess(InPawn);

    GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}
