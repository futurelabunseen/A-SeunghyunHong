# SeunghyunHong

```cpp
// Share/EditorNames.h
...

namespace GOTagNames
{
	const static FName RED_TEAM_PLAYER_START = FName("RedTeam");
	const static FName BLUE_TEAM_PLAYER_START = FName("BlueTeam");
}

namespace GOCollisionChannel
{
	constexpr static ECollisionChannel ATTACK_CHANNEL = ECollisionChannel::ECC_GameTraceChannel1;
	constexpr static ECollisionChannel PROJECTILE_CHANNEL = ECollisionChannel::ECC_GameTraceChannel2;
}

namespace GOConsts
{
	constexpr uint8 PLAYER_MAX_NUM_SYSTEM = 4;
	constexpr float NO_HP = 0.f;
	constexpr float STUN_EVENT_DELAY_TIME = 1.f;
	constexpr float SELF_MOVE_AMOUNT = 1.f;
	constexpr float DEAD_EVENT_DELAY_TIME = 5.f;
}

...

```

---


```cpp
// GOSkillBase.h
private:
    TObjectPtr<AActor> CachedSkillOwner;

// GOSkillBase.cpp
void UGOSkillBase::SetSkillOwner(AActor* NewOwner)
{
    if (CachedSkillOwner != NewOwner)
    {
        CachedSkillOwner = NewOwner;
        SkillOwnerCharacter = NewOwner;
        if (AActor* OwnerActor = Cast<AActor>(SkillOwnerCharacter))
        {
            ...
        }
    }
}
```

```cpp
// GOPlayerController.h
private:
    TObjectPtr<AGOPlayerState> CachedPlayerState;

// GOPlayerController.cpp
void AGOPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (CachedPlayerState != GetPlayerState<AGOPlayerState>())
    {
        CachedPlayerState = GetPlayerState<AGOPlayerState>();
        if (CachedPlayerState)
        {
            ...
        }
    }
}
```

```cpp
// GOCharacterBase.h
private:
    TObjectPtr<UGOCharacterMovementComponent> CachedMovementComponent;

// GOCharacterBase.cpp 
void AGOCharacterBase::BeginPlay()
{
    Super::BeginPlay();

    if (!CachedMovementComponent)
    {
        CachedMovementComponent = FindComponentByClass<UGOCharacterMovementComponent>();
        if (CachedMovementComponent)
        {
            ...
        }
    }
}
```

---


```cpp
class AGOBattleGameMode : public AGameModeBase
{
    // 가상 함수 선언
    virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn);
    ...
};
```


```cpp
class AGOTeamBattleGameMode : public AGOBattleGameMode
{
    // 부모 클래스의 가상 함수를 오버라이드하여 팀 기반 로직 추가
    virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn) override;
    ...
};
```

```cpp
void AGOBattleGameMode::OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn)
{
    AGOPlayerState* AttackerPlayerState = Killer ? Cast<AGOPlayerState>(Killer->PlayerState) : nullptr;
    AGOPlayerState* VictimPlayerState = KilledPlayer ? Cast<AGOPlayerState>(KilledPlayer->PlayerState) : nullptr;
    AGOGameState* GOGameState = GetGameState<AGOGameState>();

    if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState)
    {
        AttackerPlayerState->AddToScore(1.0f);
        GOGameState->UpdateTopscore(AttackerPlayerState);
    }

    if (VictimPlayerState)
    {
        VictimPlayerState->AddToDefeats(1);
    }

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AGOPlayerController* GOPlayerController = Cast<AGOPlayerController>(*It);
        if (GOPlayerController && AttackerPlayerState && VictimPlayerState)
        {
            GOPlayerController->BroadcastElim(AttackerPlayerState, VictimPlayerState);
        }
    }
}
```

```cpp
void AGOTeamBattleGameMode::OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn)
{
    // 부모 클래스의 기본 사망 처리 로직 호출
    Super::OnPlayerKilled(Killer, KilledPlayer, KilledPawn);

    // 추가적인 팀 점수 업데이트 로직
    AGOGameState* BGameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(this));
    AGOPlayerState* AttackerPlayerState = Killer ? Cast<AGOPlayerState>(Killer->PlayerState) : nullptr;
    if (BGameState && AttackerPlayerState)
    {
        if (AttackerPlayerState->GetTeamType() == ETeamType::ET_BlueTeam)
        {
            BGameState->BlueTeamScores();
        }
        if (AttackerPlayerState->GetTeamType() == ETeamType::ET_RedTeam)
        {
            BGameState->RedTeamScores();
        }
    }
}
```

```cpp
float AGOPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead) return 0;

    GOBattleGameMode = GOBattleGameMode == nullptr ? GetWorld()->GetAuthGameMode<AGOBattleGameMode>() : GOBattleGameMode;
    if (GOBattleGameMode == nullptr) return 0.0f;

    DamageAmount = GOBattleGameMode->CalculateDamage(EventInstigator, Controller, DamageAmount);
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // HP가 0 이하일 경우 사망 처리
    if (Stat->GetCurrentHp() <= GOConsts::NO_HP)
    {
        IGOBattleInterface* GOBattleMode = GetWorld()->GetAuthGameMode<IGOBattleInterface>();
        if (GOBattleMode)
        {
            GOBattleMode->OnPlayerKilled(EventInstigator, GetController(), this);
            AController* KillerController = EventInstigator;
            AGOPlayerState* VictimPlayerState = GetPlayerState<AGOPlayerState>();
            HandlePlayerKilled(KillerController, VictimPlayerState);
        }
    }

    ...

    return ActualDamage;
}

```
---

```cpp
void UGOSkillSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ...

    if (ToolTipClass)
    {
        SkillToolTipWidget = CreateWidget<UGOSkillSlotToolTipWidget>(this, ToolTipClass);
        SkillToolTipWidget->BattleSkillSlotBeingHobered = this;
        SetToolTip(SkillToolTipWidget);
    }
}

...

void UGOSkillSlotWidget::BindSkill(UGOSkillBase* Skill)
{
    ...
    CurrentSkill = Skill;
    SkillToolTipWidget->UpdateSkillToolTip(CurrentSkill->GetTotalSkillData().SkillStatName);
}
...
```

```cpp
void UGOSkillSlotToolTipWidget::UpdateSkillToolTip(FName SkillName)
{
    ...
    FGOSkillData* SkillData = GOGameSubsystem->GetSkillData(SkillName);
    if (SkillData)
    {
        // 스킬의 이름, 유형 등의 정보 표시
        SkillNameText->SetText(FText::FromString(SkillData->SkillName));
        ...

        FGOSkillStat* SkillStat = GameSubsystem->GetSkillStatData(SkillData->SkillStatName);
        if (SkillStat)
        {
            // 재사용 대기 시간, 마나 비용 등의 정보 표시
            CoolTimeText->SetText(FText::AsNumber(SkillStat->CoolDownTime));
            ...
        }
    }
}
```

---


```c
float2 Coord = float2(UV);  // UV 좌표를 float2로 저장
Coord = (Coord - float2(0.5,0.5))*2;  // 좌표를 (0,0) 중심으로 이동시키고 2배로 확장
Coord = float2(-Coord.y, Coord.x);  // 좌표를 90도 반시계 방향으로 회전

float PercentRad = radians(percent*360);  // 퍼센트를 라디안 각도로 변환
float3 pointer = float3(cos(PercentRad), sin(PercentRad), 0);  // 퍼센트 각도에 해당하는 방향 벡터를 계산

float3 Coord3d = float3(Coord.x, Coord.y, 0);  // 2D 좌표를 3D 벡터로 변환
float3 Cross = cross(Coord3d, pointer);  // 현재 좌표와 방향 벡터의 외적을 계산

float returnVal = ShadeVal;  // 초기 쉐이드 값을 ShadeVal로 설정

if (pointer.y > 0)  // 포인터가 상반부에 있는지 확인
{
    if (Cross.z > 0 && Coord.y > 0)  // 상반부의 특정 조건을 만족하는 경우
    {
        returnVal = 1;  
    }
}
else  // 포인터가 하반부에 있는 경우
{
    if (Cross.z > 0 || Coord.y > 0)  // 하반부의 특정 조건을 만족하는 경우
    {
        returnVal = 1;  
    }
}

return returnVal; 
```


```cpp
void UGOSkillSlotWidget::NativeTick(const FGeometry& Geometry, float DeltaSeconds)
{
    Super::NativeTick(Geometry, DeltaSeconds);

    if (bIsCooldownActive && CurrentSkill)
    {
        float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
        float TimeElapsed = CurrentTime - CooldownStartTime;
        float CooldownDuration = CurrentSkill->GetCoolDownTime();
        float RemainingTime = CooldownDuration - TimeElapsed;

        // 쿨타임 완료
        if (RemainingTime <= 0)
        {
            bIsCooldownActive = false;
            CurrentSkill->SetIsOnCoolTime(false);
            OnCooldownChanged(false);
        }
        else
        {
            // 쿨다운 시간 업데이트
            FString FormattedTime = FString::Printf(TEXT("%.1f"), RemainingTime);
            CooldownText->SetText(FText::FromString(FormattedTime));

            if (MatInstance)
            {
                float Percent = 1.0f - (RemainingTime / CooldownDuration);
                MatInstance->SetScalarParameterValue(GOMaterial::PERCENT, Percent);
            }
        }
    }
}
```
---

```cpp
void UGOHUDWidget::AddElimAnnouncement(FString Attacker, FString Victim)
{
    ...

    // 킬로그 위젯 생성
    UGOElimAnnouncementWidget* ElimAnnouncementWidget =
        CreateWidget<UGOElimAnnouncementWidget>(OwningPlayer, GOElimAnnouncementClass);

    if (!ElimAnnouncementWidget)
    {
        return;
    }

    // 위젯 텍스트 설정 및 뷰포트에 추가
    ElimAnnouncementWidget->SetElimAnnouncementText(Attacker, Victim);
    ElimAnnouncementWidget->AddToViewport();

    // 기존 메시지를 위로 이동
    ShiftExistingMessages();

    // 새 메시지를 리스트에 추가
    ElimMessages.Add(ElimAnnouncementWidget);

    // 일정 시간 후 메시지를 제거하도록 타이머 설정
    ScheduleElimMessageRemoval(ElimAnnouncementWidget, OwningPlayer);
}

// 기존 킬로그 메시지들을 위로 이동시키는 함수
void UGOHUDWidget::ShiftExistingMessages()
{
    for (UGOElimAnnouncementWidget* Msg : ElimMessages)
    {
        if (Msg && Msg->AnnouncementBox)
        {
            UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Msg->AnnouncementBox);
            if (CanvasSlot)
            {
                FVector2D NewPosition(CanvasSlot->GetPosition().X, CanvasSlot->GetPosition().Y - CanvasSlot->GetSize().Y);
                CanvasSlot->SetPosition(NewPosition);
                UE_LOG(LogTemp, Warning, TEXT("CanvasSlot SetPosition"));
            }
        }
    }
}

// 킬로그 메시지를 일정 시간 후 제거하도록 타이머를 설정하는 함수
void UGOHUDWidget::ScheduleElimMessageRemoval(UGOElimAnnouncementWidget* ElimAnnouncementWidget, AGOPlayerController* OwningPlayer)
{
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

```
---

```cpp
// GOPlayerController.h
TScriptInterface<IGOHighlightInterface> LastActor;
TScriptInterface<IGOHighlightInterface> ThisActor;
FHitResult CursorHit;

// GOPlayerController.cpp
void AGOPlayerController::CursorTrace()
{
    // 커서 아래의 히트 결과 가져오기
    GetHitResultUnderCursor(CCHANNEL_GOACTION, false, CursorHit);
    if (!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();

    // ThisActor가 플레이어 캐릭터인지 확인
    AGOPlayerCharacter* ThisPlayerCharacter = Cast<AGOPlayerCharacter>(ThisActor.GetObject());

    if (ThisPlayerCharacter)
    {
        AGOPlayerState* ThisPlayerState = ThisPlayerCharacter->GetPlayerState<AGOPlayerState>();
        AGOPlayerState* LocalPlayerState = GetPlayerState<AGOPlayerState>();

        // 상대 팀 여부 확인
        if (ThisPlayerState && LocalPlayerState && ThisPlayerState->GetTeamType() != LocalPlayerState->GetTeamType())
        {
            // ThisActor가 상대 팀일 때만 하이라이트 처리
            if (LastActor != ThisActor)
            {
                if (LastActor)
                {
                    LastActor->UnHighlightActor();
                }
                if (ThisActor)
                {
                    ThisActor->HighlightActor();
                }
            }
        }
        else
        {
            // 같은 팀이거나 팀 정보가 없을 때 하이라이트 제거
            if (LastActor)
            {
                LastActor->UnHighlightActor();
            }
        }
    }
    else
    {
        // ThisActor가 플레이어 캐릭터가 아니면 하이라이트 제거
        if (LastActor)
        {
            LastActor->UnHighlightActor();
        }
    }
}

```
---
```cpp
// AGOPlayerCharacter.cpp
void AGOPlayerCharacter::OnInputStarted()
{
    if (InputSubsystem->GetCurrentInputType() == ECommonInputType::MouseAndKeyboard)
    {
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void AGOPlayerCharacter::OnSetDestinationTriggered()
{
    // 현재 입력 타입이 마우스와 키보드인 경우
    if (InputSubsystem->GetCurrentInputType() == ECommonInputType::MouseAndKeyboard)
    {
        FollowTime += GetWorld()->GetDeltaSeconds();
        const APlayerController* controller = Cast<APlayerController>(GetController());

        FHitResult Hit;
        bool bHitSuccessful = controller->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

        if (bHitSuccessful)
        {
            CachedDestination = Hit.Location;
        }

        FVector WorldDirection = (CachedDestination - GetActorLocation()).GetSafeNormal();
        AddMovementInput(WorldDirection, GOConsts::SELF_MOVE_AMOUNT, false);
    }
}

void AGOPlayerCharacter::OnSetDestinationReleased()
{
    // 현재 입력이 FollowTime이 짧은 경우
    if (InputSubsystem->GetCurrentInputType() == ECommonInputType::MouseAndKeyboard && FollowTime <= ShortPressThreshold)
    {
        // 네비게이션 경로 찾기
        if (UNavigationPath* NavPath =
              UNavigationSystemV1::FindPathToLocationSynchronously(this, GetActorLocation(), CachedDestination))
        {
            CachedDestination = NavPath->PathPoints.Last();
            bSelfMove = true;
        }
    }
    FollowTime = 0.f;
}

// 입력 바인딩 설정
void AGOPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    EnhancedInputComponent->BindAction(ActionClickSetDestination,
        ETriggerEvent::Started, this, &AGOPlayerCharacter::OnInputStarted);
    EnhancedInputComponent->BindAction(ActionClickSetDestination,
        ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSetDestinationTriggered);
    EnhancedInputComponent->BindAction(ActionClickSetDestination,
        ETriggerEvent::Completed, this, &AGOPlayerCharacter::OnSetDestinationReleased);
    EnhancedInputComponent->BindAction(ActionClickSetDestination,
        ETriggerEvent::Canceled, this, &AGOPlayerCharacter::OnSetDestinationReleased);
}

```

---
UGOSkillBase 클래스에서 ITargetable 인터페이스를 사용하여 타겟을 탐지
```cpp
// GOSkillBase.h
UCLASS(Blueprintable)
class GUARDIANSORDERS_API UGOSkillBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void HandleSkillTrigger();

    	...
protected:
	...
	virtual void HandleAutoTargetRadius();
	virtual void HandleAutoTargetRadiusDegree();

	ITargetable* DetectClosestTarget(float Radius);
	ITargetable* DetectClosestTargetRadiusDegreeBase(const FVector2D& Direction, float Radius, float Degree);
};

// GOSkillBase.cpp
void UGOSkillBase::HandleSkillTrigger()
{
    switch (GetSkillTriggerType())
    {
    case ESkillTriggerType::NonTarget:
        break;
    case ESkillTriggerType::AutoTargetRadius:
        HandleAutoTargetRadius();
        break;
    case ESkillTriggerType::AutoTargetRadiusDegree:
        HandleAutoTargetRadiusDegree();
        break;
    default:
        break;
    }
}

void UGOSkillBase::HandleAutoTargetRadius()
{
    float Radius = GetAutoDetectionRadius();
    ITargetable* ClosestTarget = DetectClosestTarget(Radius);
    SetTarget(Cast<AActor>(ClosestTarget));
}

void UGOSkillBase::HandleAutoTargetRadiusDegree()
{
    float Radius = GetAutoDetectionRadius();
    float Degree = GetAutoDetectionDegree();
    FVector ForwardVector = GetSkillOwner()->GetActorForwardVector();
    FVector2D Direction(ForwardVector.X, ForwardVector.Y);
    ITargetable* ClosestTarget = DetectClosestTargetRadiusDegreeBase(Direction, Radius, Degree);
    SetTarget(Cast<AActor>(ClosestTarget));
}

// 주어진 반경 내에서 반경 내의 모든 타겟을 검사 후, 각 타겟과의 거리를 계산해 가장 가까운 타겟을 탐지
ITargetable* UGOSkillBase::DetectClosestTarget(float Radius)
{
    TArray<FOverlapResult> OutResults;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetSkillOwner());

    FVector Location = GetSkillOwner()->GetActorLocation();
    GetWorld()->OverlapMultiByChannel(OutResults, Location, FQuat::Identity,
	GOCollisionChannel::ATTACK_CHANNEL, FCollisionShape::MakeSphere(Radius), CollisionParams);

    ITargetable* ClosestTarget = nullptr;
    float MinDistance = FLT_MAX;

    for (auto& Result : OutResults)
    {
        AActor* HitActor = Result.GetActor();
        ITargetable* Target = Cast<ITargetable>(HitActor);
        if (Target && HitActor != GetSkillOwner())
        {
            float Distance = (Target->GetTargetLocation() - Location).Size();
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                ClosestTarget = Target;
            }
        }
    }

    return ClosestTarget;
}

// 각 타겟과의 방향 벡터를 계산하고, 내적(Dot Product)을 이용하여 각도 조건을 만족하는 타겟을 탐지
ITargetable* UGOSkillBase::DetectClosestTargetRadiusDegreeBase(const FVector2D& Direction, float Radius, float Degree)
{
    TArray<FOverlapResult> OutResults;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetSkillOwner());

    FVector Location = GetSkillOwner()->GetActorLocation();
    GetWorld()->OverlapMultiByChannel(OutResults, Location, FQuat::Identity,
	GOCollisionChannel::ATTACK_CHANNEL, FCollisionShape::MakeSphere(Radius), CollisionParams);

    ITargetable* ClosestTarget = nullptr;
    float MinDistance = FLT_MAX;

    for (auto& Result : OutResults)
    {
        AActor* HitActor = Result.GetActor();
        ITargetable* Target = Cast<ITargetable>(HitActor);
        if (Target && HitActor != GetSkillOwner())
        {
            FVector2D HitDir = FVector2D(Target->GetTargetLocation() - Location);
            HitDir.Normalize();
            float CosTheta = FMath::Cos(FMath::DegreesToRadians(Degree));
            if (FVector2D::DotProduct(Direction, HitDir) >= CosTheta)
            {
                float Distance = (Target->GetTargetLocation() - Location).Size();
                if (Distance < MinDistance)
                {
                    MinDistance = Distance;
                    ClosestTarget = Target;
                }
            }
        }
    }

    return ClosestTarget;
}

```

---

```cpp
// GOGameState.h
UPROPERTY(ReplicatedUsing = OnRep_ShowSelectedHeroWidget)
bool bShowHeroSelectionWidget;

UFUNCTION()
void OnRep_ShowSelectedHeroWidget();

// GOGameState.cpp
void AGOGameState::OnRep_ShowSelectedHeroWidget()
{
	ShowSelectedHeroWidget();
}

void AGOGameState::OnGamePlayerReadyNotified()
{
	if (HasAuthority())
	{
		bShowHeroSelectionWidget = true;
		ShowSelectedHeroWidget();
	}
}

void AGOGameState::ShowSelectedHeroWidget()
{
	LobbyPlayerController->DisplayHeroSelectionWidget();
}
```
```cpp
// GOLobbyGameMode.cpp
void AGOLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    SetupInputMode(NewPlayer);

    HandleNewPlayerLogin(NewPlayer); // 새로운 플레이어 로그인 처리
}

void AGOLobbyGameMode::HandleNewPlayerLogin(APlayerController* NewPlayer)
{
    AssignPlayerToTeam(NewPlayer); // 플레이어를 팀에 할당
}

void AGOLobbyGameMode::AssignPlayerToTeam(APlayerController* NewPlayer)
{
    if (GOGameState)
    {
        AGOPlayerState* NewPlayerState = NewPlayer->GetPlayerState<AGOPlayerState>();
        if (NewPlayerState && NewPlayerState->GetTeamType() == ETeamType::ET_NoTeam)
        {
            // 팀을 결정하고 플레이어 상태를 업데이트
            ETeamType AssignedTeam = DetermineTeam(GOGameState);
            NewPlayerState->SetTeam(AssignedTeam);
            AddPlayerToTeam(GOGameState, NewPlayerState, AssignedTeam); // 팀에 플레이어 추가
        }
    }
}

ETeamType AGOLobbyGameMode::DetermineTeam(AGOGameState* GOGameState) const
{
    // 팀의 현재 인원수를 기준으로 팀을 결정
    return (GOGameState->BlueTeam.Num() >= GOGameState->RedTeam.Num()) ? ETeamType::ET_RedTeam : ETeamType::ET_BlueTeam;
}

void AGOLobbyGameMode::AddPlayerToTeam(AGOGameState* GOGameState, AGOPlayerState* NewPlayerState, ETeamType Team)
{
    // 플레이어를 지정된 팀에 추가
    if (Team == ETeamType::ET_RedTeam)
    {
        GOGameState->RedTeam.AddUnique(NewPlayerState);
    }
    else
    {
        GOGameState->BlueTeam.AddUnique(NewPlayerState);
    }
}

```


---
연마석

```cpp
void AGOPlayerState::AddKilledEnemyPlayer(int32 pID)
{
    KilledEnemyPlayers.Add(pID);
    CheckForGrindingStone(); // 적 플레이어를 추가할 때마다 연마석 조건을 확인
}

bool AGOPlayerState::HasKilledAllEnemyPlayers(const TArray<int32>& EnemyPlayerIds) const
{
    if (KilledEnemyPlayers.Num() != GOConsts::TEAM_MAX_NUM_SYSTEM) // 적 플레이어를 2명 죽였는지 확인
        return false;
    for (int32 EnemyPlayerId : EnemyPlayerIds)
    {
        if (!KilledEnemyPlayers.Contains(EnemyPlayerId))
        {
            return false;
        }
    }
    return true;
}

void AGOPlayerState::CheckForGrindingStone()
{
	const TArray<int32>& EnemyPlayerIds =
		(GetTeamType() == ETeamType::ET_RedTeam) ? GOGameState->BlueTeamPlayerIds : GOGameState->RedTeamPlayerIds;
        if (HasKilledAllEnemyPlayers(EnemyPlayerIds))
        {
            SetGrindingStone(true); // 연마석 활성화
            ResetKilledEnemyPlayers(); // 적 플레이어 리스트 초기화
        }
    }
}

void AGOPlayerState::SetGrindingStone(bool bNewGrindingStone)
{
    if (bHasGrindingStone != bNewGrindingStone)
    {
        if (HasAuthority())
        {
		bHasGrindingStone = bNewGrindingStone;
		GOPlayerController->SetGrindingStoneVisible();
        }
    }
}

void AGOPlayerState::OnRep_HasGrindingStone()
{
	GOPlayerController->SetGrindingStoneVisible();
}

void AGOPlayerState::ResetKilledEnemyPlayers()
{
    KilledEnemyPlayers.Empty();
    
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
	TimerHandle, this, &AGOPlayerState::ResetGrindingStone,
	GOConsts::GRINDINGSTONE_EVENT_DELAY_TIME, false); // 2초 후 연마석 리셋
}


```
