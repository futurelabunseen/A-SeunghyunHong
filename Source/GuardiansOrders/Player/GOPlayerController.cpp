// Fill out your copyright notice in the Description page of Project Settings.


#include "GOPlayerController.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "UI/GOHUDWidget.h"
#include "CommonActivatableWidget.h"
#include "UI/SkillWidget/GOSkillSetBarWidget.h"
#include <Game/GOPlayerState.h>
#include "Game/GOGameState.h"
#include "Game/GOTeamBattleGameMode.h"
#include <GameData/GOGameSubsystem.h>
#include <Character/GOPlayerCharacter.h>
#include "UI/GOBattleCharacterOverlayWidget.h"
#include "CommonTextBlock.h"
#include "UI/GOTeamMemberWidget.h"
#include <Kismet/GameplayStatics.h>
#include "Physics/GOCollision.h"
#include "CircleSpell/GOMagicCircle.h"
#include "Components/DecalComponent.h"
#include "Components/Overlay.h"
#include "CommonUserWidget.h"
#include "GameFramework/GameMode.h" // AGameModeBase를 포함
#include "Net/UnrealNetwork.h"
#include "UI/GOGrindingStoneWidget.h"
#include "UI/GOReturnToMainWidget.h"
#include "Components/Image.h"

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

    if (MatchState == MatchState::Cooldown)
    {
        CursorTrace();
    }

}

void AGOPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AGOPlayerController, MatchState);
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
    CheckMatchState();

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
            if(GOHUDWidget->GrindingStoneWidget != nullptr)
                UE_LOG(LogTemp, Warning, TEXT("GrindingStoneWidget is not null"));

            if (GOHUDWidget->CharacterOverlayClass)
            {
                UE_LOG(LogTemp, Warning, TEXT("AddCharacterOverlay -1"));

                // GOHUDWidget->AddCharacterOverlay();
                
                
                //GetWorld()->GetTimerManager().SetTimer(CharacterOverlayTimerHandle, this, &AGOPlayerController::AddCharacterOverlayDelayed, 5.0f, false);
                
                InitTeamScores(); // TODO : 구조
            }

        }

    }

    UE_LOG(LogTemp, Warning, TEXT("[Time]"));

    // 5초 후에 매치 카운트다운 시작
    GetWorld()->GetTimerManager().SetTimer(MatchStartTimerHandle, this, &AGOPlayerController::StartMatchCountdown, 5.0f, false);
    UE_LOG(LogTemp, Warning, TEXT("AGOPlayerController::BeginPlay()"));

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
        AGOPlayerState* PS = GetPlayerState<AGOPlayerState>();
        if (PS)
        {
            PS->SetNickname(PS->SelectedHero.PlayerName);
            if (PS->GetPawn())
            {
                AGOCharacterBase* Character = Cast<AGOCharacterBase>(PS->GetPawn());
                if (Character)
                {
                    //Character->UpdateNicknameWidget(PS->SelectedHero.PlayerName);
                }
            }
        }
        }, 5.0f, false);
    
    // Set other players' names on the listen server
    //if (IsLocalController() && GetNetMode() == NM_ListenServer)
    //{
        //for (APlayerState* PS : GOBattleGameState->PlayerArray)
        //{
        //    AGOPlayerState* GOPlayerState = Cast<AGOPlayerState>(PS);
        //    if (GOPlayerState && GOPlayerState != GetPlayerState<AGOPlayerState>())
        //    {
        //        if (GOPlayerState->GetPawn())
        //        {
        //            AGOCharacterBase* C = Cast<AGOCharacterBase>(GOPlayerState->GetPawn());
        //            if (C)
        //            {
        //                C->UpdateNicknameWidget(GOPlayerState->SelectedHero.PlayerName);
        //            }
        //        }
        //    }
        //}
    //}
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

void AGOPlayerController::SetGrindingStoneVisible()
{
    if(IsLocalPlayerController())
    { 
    if (GOHUDWidget)
    {
        bool bHUDVaild = GOHUDWidget &&
            GOHUDWidget->GrindingStoneWidget;

        UE_LOG(LogTemp, Warning, TEXT("GrindingStoneWidget : %d"), bHUDVaild);

        if (bHUDVaild)
        {
            UE_LOG(LogTemp, Warning, TEXT("GrindingStoneWidget : %s"), *GOHUDWidget->GrindingStoneWidget->GetOwningPlayer()->GetName());

            GOHUDWidget->GrindingStoneWidget->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("GrindingStoneWidget Visible"));
        }
    }

    else
    {
        // 클라는 이게 null
        UE_LOG(LogTemp, Warning, TEXT("GrindingStoneWidget : GOHUDWidget is null"));

    }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GrindingStoneWidget : is not IsLocalPlayerController"));

    }
}

void AGOPlayerController::SetHUDWinnerText(const FString& WinnerText)
{
    if (GOHUDWidget && GOHUDWidget->WinnerText)
    {
        GOHUDWidget->ReturnToMainBGImage->SetVisibility(ESlateVisibility::Visible);
        GOHUDWidget->WidgetReturnToMainMenu->SetVisibility(ESlateVisibility::Visible);

        GOHUDWidget->WinnerText->SetVisibility(ESlateVisibility::Visible);
        GOHUDWidget->WinnerText->SetText(FText::FromString(WinnerText));
    }
}

void AGOPlayerController::BroadcastElim(AGOPlayerState* Attacker, AGOPlayerState* Victim)
{
    UE_LOG(LogTemp, Warning, TEXT("BroadcastElim 0: %s"));

    ClientElimAnnouncement(Attacker, Victim);
}

void AGOPlayerController::ClientElimAnnouncement_Implementation(AGOPlayerState* Attacker, AGOPlayerState* Victim)
{
    AGOPlayerState* Self = GetPlayerState<AGOPlayerState>();
    if (Attacker && Victim && Self)
    {
        //UE_LOG(LogTemp, Warning, TEXT("BroadcastElim 1 : %s"));

        if (GOHUDWidget)
        {
            //UE_LOG(LogTemp, Warning, TEXT("BroadcastElim 1 : %s"));

            if (Attacker == Self && Victim != Self)
            {
                GOHUDWidget->AddElimAnnouncement(Self->SelectedHero.PlayerName, Victim->SelectedHero.PlayerName);
                return;
            }
            if (Victim == Self && Attacker != Self)
            {
                GOHUDWidget->AddElimAnnouncement(Attacker->SelectedHero.PlayerName, Self->SelectedHero.PlayerName);
                return;
            }
            if (Attacker == Victim && Attacker == Self)
            {
                return;
            }
            if (Attacker == Victim && Attacker != Self)
            {
                return;
            }
            else if (Attacker != Self && Victim != Self)
            {
                GOHUDWidget->AddElimAnnouncement(Attacker->SelectedHero.PlayerName, Victim->SelectedHero.PlayerName);
            }
        }
    }
}

void AGOPlayerController::StartMatchCountdown()
{
    MatchTime = 180.F; // 매치 시간 설정

    SetHUDMatchCountdown(MatchTime);
}

bool AGOPlayerController::CheckMatchState()
{
    if (MatchState == MatchState::Cooldown) return true;
    
    return false;
}

void AGOPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AGOPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // SetHUDTime();
    // CheckTimeSync(DeltaTime);
    if (MatchState == MatchState::Cooldown)
    {
        if (CountdownTime > 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("[MatchState] AGOTeamBattleGameMode Cooldown Countdown 0: %f"), CountdownTime);

            CountdownTime -= DeltaTime;
            SetHUDMatchCountdown(CountdownTime);
        }
    }
}

float AGOPlayerController::GetServerTime()
{
    if (MatchState != MatchState::Cooldown) return 0.f;
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

void AGOPlayerController::OnMatchStateSet(FName State)
{
    MatchState = State;

    if (MatchState == MatchState::InProgress) // InProgress 전의 커스텀 스테이트 필요 
    {
        //// 5초 후에 Ready State로 넘어가도록 하기
        //if (IsLocalPlayerController() && IsValid(GOHUDWidgetClass))
        //{
        //    GOHUDWidget = CreateWidget<UGOHUDWidget>(this, GOHUDWidgetClass);

        //    if (GOHUDWidget)
        //    {
        //        GOHUDWidget->AddCharacterOverlay();
        //    }

        //}
    }
    else if (MatchState == MatchState::Cooldown)
    {
        // 5초 후에 Ready State로 넘어가도록 하기
        if (IsLocalPlayerController() && IsValid(GOHUDWidgetClass))
        {
            //GOHUDWidget = CreateWidget<UGOHUDWidget>(this, GOHUDWidgetClass);

            if (GOHUDWidget)
            {
                UE_LOG(LogTemp, Warning, TEXT("AGOPlayerController::OnMatchStateSet "));

                // GOHUDWidget->AddCharacterOverlay();
                AddCharacterOverlayDelayed();
            }

        }
        //AGOGameState* GOGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
        //TArray<APlayerState*> PlayerStates = GOGameState->PlayerArray;
        //for (APlayerState* GOPlayerState : PlayerStates)
        //{
        //    if (GOPlayerState)
        //    {
        //        if (AGOPlayerController* PlayerController = Cast<AGOPlayerController>(GOPlayerState->GetOwner()))
        //        {
        //            AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
        //            if (BGameState)
        //            {
        //                AGOPlayerState* BPState = PlayerController->GetPlayerState<AGOPlayerState>();
        //                if (BPState && BPState->GetTeamType() == ETeamType::ET_BlueTeam)
        //                {
        //                    UE_LOG(LogTemp, Warning, TEXT("[SetTeamColor]Cooldown::SetTeamColor"));

        //                    BPState->SetTeamColor(ETeamType::ET_BlueTeam);

        //                }
        //                else if (BPState && BPState->GetTeamType() == ETeamType::ET_RedTeam)
        //                {
        //                    BPState->SetTeamColor(ETeamType::ET_RedTeam);

        //                }
        //            }
        //        }
        //    }
        //}
    }
    else if (MatchState == MatchState::RoundEnd)
    {
        UE_LOG(LogTemp, Warning, TEXT("[RoundEnd] AGOPlayerController::OnMatchStateSet RoundEnd %s"), *this->GetName());

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
    if (IsLocalPlayerController() && GOHUDWidget)
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

void AGOPlayerController::SetHUDMatchCountdown(float CountdownTimeSec)
{
    UE_LOG(LogTemp, Warning, TEXT("[MatchState] AGOPlayerController FConstPlayerControllerIterator: %f, %s"), CountdownTime, *this->GetName());
    if (GOHUDWidget)
    {
        bool bHUDVaild = GOHUDWidget &&
            GOHUDWidget->CharacterOverlay &&
            GOHUDWidget->CharacterOverlay->MatchCountdownText;

        if (bHUDVaild)
        {
            if (CountdownTime <= 0.f)
            {
                CountdownTime = 0.f; // 카운트다운 시간을 0으로 설정

                GOHUDWidget->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(TEXT(" END ")));
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
    float TimeLeft = 0.f;
    if (MatchState == MatchState::Cooldown)
        TimeLeft = MatchTime - GetServerTime() + 5.f;
    uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetServerTime());
    if (MatchState == MatchState::Cooldown && CountdownInt != SecondsLeft)
    {
        //SetHUDMatchCountdown(MatchTime - GetServerTime());
        SetHUDMatchCountdown(TimeLeft);
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
    if (!CursorHit.bBlockingHit) return;
    GetHitResultUnderCursor(CCHANNEL_GOACTION, false, CursorHit);

    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();

    // ThisActor가 플레이어 캐릭터인지 확인
    AGOPlayerCharacter* ThisPlayerCharacter = Cast<AGOPlayerCharacter>(ThisActor.GetObject());

    // ThisActor가 플레이어 캐릭터라면 팀 정보 확인
    if (ThisPlayerCharacter)
    {
        AGOPlayerState* ThisPlayerState = ThisPlayerCharacter->GetPlayerState<AGOPlayerState>();
        AGOPlayerState* LocalPlayerState = GetPlayerState<AGOPlayerState>();

        if (ThisPlayerState && LocalPlayerState && ThisPlayerState->GetTeamType() != LocalPlayerState->GetTeamType())
        {
            // ThisPlayerCharacter가 상대팀일 때만 하이라이트 처리
            if (LastActor == nullptr)
            {
                if (ThisActor != nullptr)
                {
                    ThisActor->HighlightActor();
                }
            }
            else
            {
                if (ThisActor == nullptr)
                {
                    LastActor->UnHighlightActor();
                }
                else
                {
                    if (LastActor != ThisActor)
                    {
                        LastActor->UnHighlightActor();
                        ThisActor->HighlightActor();
                    }
                }
            }
        }
        else
        {
            // 팀 정보가 없거나 같은 팀일 경우 하이라이트를 제거
            if (LastActor != nullptr)
            {
                LastActor->UnHighlightActor();
            }
        }
    }
    else
    {
        // ThisActor가 플레이어 캐릭터가 아니면 하이라이트를 제거
        if (LastActor != nullptr)
        {
            LastActor->UnHighlightActor();
        }
    }

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

    //if (LastActor == nullptr)
    //{
    //    if (ThisActor != nullptr)
    //    {
    //        // Case B
    //        ThisActor->HighlightActor();
    //    }
    //    else
    //    {
    //        // Case A - both are null, do nothing
    //    }
    //}
    //else // LastActor is valid
    //{
    //    if (ThisActor == nullptr)
    //    {
    //        // Case C
    //        LastActor->UnHighlightActor();
    //    }
    //    else // both actors are valid
    //    {
    //        if (LastActor != ThisActor)
    //        {
    //            // Case D
    //            LastActor->UnHighlightActor();
    //            ThisActor->HighlightActor();
    //        }
    //        else
    //        {
    //            // Case E - do nothing
    //        }
    //    }
    //}

}

void AGOPlayerController::OnRep_MatchState()
{
    if (MatchState == MatchState::InProgress) // InProgress 전의 커스텀 스테이트 필요 
    {
        //if (IsLocalPlayerController() && IsValid(GOHUDWidgetClass))
        //{
        //    GOHUDWidget = CreateWidget<UGOHUDWidget>(this, GOHUDWidgetClass);

        //    if (GOHUDWidget)
        //    {
        //        GOHUDWidget->AddCharacterOverlay();
        //    }

        //}
    }
    else if (MatchState == MatchState::Cooldown)
    {
        if (IsValid(GOHUDWidgetClass))
        {
            //GOHUDWidget = CreateWidget<UGOHUDWidget>(this, GOHUDWidgetClass);

            if (GOHUDWidget)
            {
                GOHUDWidget->AddCharacterOverlay();

                // SetHUDMatchCountdown(180);
            }

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
    else if (MatchState == MatchState::RoundEnd)
    {
        UE_LOG(LogTemp, Warning, TEXT("[RoundEnd] AGOPlayerController 0"));

        GOHUDWidget->WidgetReturnToMainMenu->SetVisibility(ESlateVisibility::Visible);

        AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
        if (BGameState)
        {
            int32 RedTeamScore = BGameState->RedTeamScore;
            int32 BlueTeamScore = BGameState->BlueTeamScore;

            FString WinnerText;

            if (RedTeamScore > BlueTeamScore)
            {
                WinnerText = TEXT("WINNER : RED");
            }
            else if (BlueTeamScore > RedTeamScore)
            {
                WinnerText = TEXT("WINNER : BLUE");
            }
            else
            {
                WinnerText = TEXT("DRAW");
            }

            if (GOHUDWidget)
            {
                SetHUDWinnerText(WinnerText);

            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[RoundEnd] AGOPlayerController null"));

            }

        }

    }
}


