// Fill out your copyright notice in the Description page of Project Settings.


#include "GOPlayerController.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "UI/GOHUDWidget.h"
#include "CommonActivatableWidget.h"
#include "UI/SkillWidget/GOSkillSetBarWidget.h"
#include <Game/GOPlayerState.h>
#include "Game/GOGameState.h"
#include <GameData/GOGameSubsystem.h>
#include <Character/GOPlayerCharacter.h>
#include "UI/GOBattleCharacterOverlayWidget.h"
#include "CommonTextBlock.h"
#include "UI/GOTeamMemberWidget.h"
#include <Kismet/GameplayStatics.h>
#include "Physics/GOCollision.h"

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

void AGOPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    CursorTrace();
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
        //if (GEngine)
        //{
        //    GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green,
        //        FString::Printf(TEXT("PostSeamlessTravel - PlayerId: %d"), PlayerId));
        //}
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

    GOBattleGameState = GetWorld()->GetGameState<AGOGameState>();

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

                // GOHUDWidget->AddCharacterOverlay();
                GetWorld()->GetTimerManager().SetTimer(CharacterOverlayTimerHandle, this, &AGOPlayerController::AddCharacterOverlayDelayed, 5.0f, false);
                
                InitTeamScores(); // TODO : 구조
            }

        }

    }
}

void AGOPlayerController::AddCharacterOverlayDelayed()
{
    if (GOHUDWidget && GOHUDWidget->CharacterOverlayClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay -1"));
        GOHUDWidget->AddCharacterOverlay();

        if (GOBattleGameState)
        {
            int32 PlayerArrayNum = GOBattleGameState->PlayerArray.Num();
            UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay PlayerArray Num: %d"), PlayerArrayNum);

            if (PlayerArrayNum > 0)
            {
                UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay PlayerArray Num!!: %d"), PlayerArrayNum);

                SetHUDMatchMembers(PlayerArrayNum);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay PlayerArray is empty."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay GOBattleGameState is null."));
        }
    }
}


void AGOPlayerController::HideTeamScores()
{
    if (GOHUDWidget)
    {
        bool bHUDVaild = GOHUDWidget &&
            GOHUDWidget->CharacterOverlay &&
            GOHUDWidget->CharacterOverlay->RedTeamScore &&
            GOHUDWidget->CharacterOverlay->BlueTeamScore;

        if (bHUDVaild)
        {
            GOHUDWidget->CharacterOverlay->RedTeamScore->SetText(FText());
            GOHUDWidget->CharacterOverlay->BlueTeamScore->SetText(FText());
        }
    }
}

void AGOPlayerController::InitTeamScores()
{
    if (GOHUDWidget)
    {
        bool bHUDVaild = GOHUDWidget &&
            GOHUDWidget->CharacterOverlay &&
            GOHUDWidget->CharacterOverlay->RedTeamScore &&
            GOHUDWidget->CharacterOverlay->BlueTeamScore;

        if (bHUDVaild)
        {
            FString Zero("0");
            GOHUDWidget->CharacterOverlay->RedTeamScore->SetText(FText::FromString(Zero));
            GOHUDWidget->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(Zero));
        }
    }
}

void AGOPlayerController::SetHUDRedTeamScore(int32 RedScore)
{
    if (GOHUDWidget)
    {
        bool bHUDVaild = GOHUDWidget &&
            GOHUDWidget->CharacterOverlay &&
            GOHUDWidget->CharacterOverlay->RedTeamScore;

        if (bHUDVaild)
        {
            FString ScoreText = FString::Printf(TEXT("%d"), RedScore);
            GOHUDWidget->CharacterOverlay->RedTeamScore->SetText(FText::FromString(ScoreText));
        }
    }
}

void AGOPlayerController::SetHUDBlueTeamScore(int32 BlueScore)
{
    if (GOHUDWidget)
    {
        bool bHUDVaild = GOHUDWidget &&
            GOHUDWidget->CharacterOverlay &&
            GOHUDWidget->CharacterOverlay->BlueTeamScore;

        if (bHUDVaild)
        {
            FString ScoreText = FString::Printf(TEXT("%d"), BlueScore);
            GOHUDWidget->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(ScoreText));
        }
    }
}

void AGOPlayerController::SetHUDMatchMembers(int32 MatchMemberNum)
{
    if (GOHUDWidget)
    {
        bool bHUDVaild = GOHUDWidget &&
            GOHUDWidget->CharacterOverlay &&
            GOHUDWidget->CharacterOverlay->RedTeamMember01 &&
            GOHUDWidget->CharacterOverlay->RedTeamMember02 &&
            GOHUDWidget->CharacterOverlay->BlueTeamMember01 &&
            GOHUDWidget->CharacterOverlay->BlueTeamMember02;

        if (bHUDVaild)
        {
            if (MatchMemberNum == 2)
            {
                UGOGameSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGOGameSubsystem>();
                if (Subsystem)
                {
                    FHeroSelectionData HeroSelectionData = Subsystem->GetHeroSelectionData();

                    UE_LOG(LogTemp, Warning, TEXT("GOBattleGameState->RedTeamHeroes: %d"), HeroSelectionData.RedTeamHeroes.Num());
                    GOHUDWidget->CharacterOverlay->RedTeamMember01->SetTeamMemberWidgetVisible(HeroSelectionData.RedTeamHeroes[0]);
                    GOHUDWidget->CharacterOverlay->BlueTeamMember01->SetTeamMemberWidgetVisible(HeroSelectionData.BlueTeamHeroes[0]);
                }
            }

            if (MatchMemberNum == 4)
            {

                UGOGameSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGOGameSubsystem>();
                if (Subsystem)
                {
                    FHeroSelectionData HeroSelectionData = Subsystem->GetHeroSelectionData();
                    
                    UE_LOG(LogTemp, Warning, TEXT("GOBattleGameState->RedTeamHeroes: %d"), HeroSelectionData.RedTeamHeroes.Num());
                    GOHUDWidget->CharacterOverlay->RedTeamMember01->SetTeamMemberWidgetVisible(HeroSelectionData.RedTeamHeroes[0]);
                    GOHUDWidget->CharacterOverlay->BlueTeamMember01->SetTeamMemberWidgetVisible(HeroSelectionData.BlueTeamHeroes[0]);
                    GOHUDWidget->CharacterOverlay->RedTeamMember02->SetTeamMemberWidgetVisible(HeroSelectionData.RedTeamHeroes[1]);
                    GOHUDWidget->CharacterOverlay->BlueTeamMember02->SetTeamMemberWidgetVisible(HeroSelectionData.BlueTeamHeroes[1]);
                }
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

void AGOPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SetHUDTime();
    CheckTimeSync(DeltaTime);
}

float AGOPlayerController::GetServerTime()
{
    if (HasAuthority()) return GetWorld()->GetTimeSeconds();
    else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void AGOPlayerController::ReceivedPlayer()
{
    Super::ReceivedPlayer();
    if (IsLocalController())
    {
        ServerRequestServerTime(GetWorld()->GetTimeSeconds());
    }
}

void AGOPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
    // TODO : GameState Custom Func
    float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
    ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AGOPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
    float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
    SingleTripTime = 0.5f * RoundTripTime;
    float CurrentServerTime = TimeServerReceivedClientRequest + SingleTripTime; // 서버가 정보를 전송하는 데 걸리는 시간까지 고려
    ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void AGOPlayerController::CheckTimeSync(float DeltaTime)
{
    TimeSyncRunningTime += DeltaTime;
    if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
    {
        ServerRequestServerTime(GetWorld()->GetTimeSeconds());
        TimeSyncRunningTime = 0.f;
    }
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

void AGOPlayerController::SetHUDDefeats(int32 Defeats)
{
    if (GOHUDWidget)
    {
        bool bHUDVaild = GOHUDWidget &&
            GOHUDWidget->CharacterOverlay &&
            GOHUDWidget->CharacterOverlay->DefeatsAmount;

        if (bHUDVaild)
        {
            FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
            GOHUDWidget->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
        }
    }
}

void AGOPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
    if (GOHUDWidget)
    {
        bool bHUDVaild = GOHUDWidget &&
            GOHUDWidget->CharacterOverlay &&
            GOHUDWidget->CharacterOverlay->MatchCountdownText;

        if (bHUDVaild)
        {
            if (CountdownTime < 0.f)
            {
                GOHUDWidget->CharacterOverlay->MatchCountdownText->SetText(FText());
                return;
            }

            int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
            int32 Seconds = CountdownTime - Minutes * 60;

            FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
            GOHUDWidget->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
        }
    }
}

void AGOPlayerController::SetHUDTime()
{
    uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetServerTime());
    if (CountdownInt != SecondsLeft)
    {
        SetHUDMatchCountdown(MatchTime - GetServerTime());
    }
    CountdownInt = SecondsLeft;
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

void AGOPlayerController::CursorTrace()
{
    UE_LOG(LogTemp, Warning, TEXT("HighlightActor AGOPlayerController CursorTrace 000000000"));

    FHitResult CursorHit;
    GetHitResultUnderCursor(CCHANNEL_GOACTION, false, CursorHit);
    if (!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();

    /**
     * LineTrace from cursor. There are several scenarios:
     * A. LastActor is null &&  ThisActor is null
     *      -> Do Nothing
     * B. LastActor is null && ThisActor is valid
     *      -> Highlight ThisActor
     * C. LastActor is valid && ThisActor is null
     *      -> UnHighlight LastActor
     * D. Both actors are valid, but LastActor != ThisActor
     *      -> UnHighlight LastActor, and Highlight ThisActor
     * E. Both actors are valid, and are the same actor
     *      -> Do Nothing
     */

    if (LastActor == nullptr)
    {
        if (ThisActor != nullptr)
        {
            // Case B
            ThisActor->HighlightActor();
        }
        else
        {
            // Case A - both are null, do nothing
        }
    }
    else // LastActor is valid
    {
        if (ThisActor == nullptr)
        {
            // Case C
            LastActor->UnHighlightActor();
        }
        else // both actors are valid
        {
            if (LastActor != ThisActor)
            {
                // Case D
                LastActor->UnHighlightActor();
                ThisActor->HighlightActor();
            }
            else
            {
                // Case E - do nothing
            }
        }
    }
}

