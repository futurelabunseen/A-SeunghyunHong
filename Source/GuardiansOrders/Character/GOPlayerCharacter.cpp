// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GOPlayerCharacter.h"
#include "UI/GOWidgetComponent.h" 
#include "UI/GOHpBarWidget.h"
#include "UI/GOManaBarWidget.h"
#include "UI/GOHUDWidget.h"
#include "CharacterStat/GOCharacterStatComponent.h"
#include "Skill/GOSkillCastComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CommonInputSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/AnimMontage.h"
#include "Physics/GOCollision.h"
#include "Engine/DamageEvents.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "Interface/GOBattleInterface.h"

AGOPlayerCharacter::AGOPlayerCharacter()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 750.0f;
	CameraBoom->CameraLagSpeed = 5.0f;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Path 
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FXImpactRef(TEXT("NiagaraSystem'/Game/AssetResource/VFX/NS_Impact.NS_Impact'"));
	if (FXImpactRef.Succeeded())
	{
		FXImpact = FXImpactRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SplineMeshRef(TEXT("StaticMesh'/Engine/EngineMeshes/Cylinder.Cylinder'"));
	if (SplineMeshRef.Succeeded())
	{
		SplineMesh = SplineMeshRef.Object;
	}

	// Input-Move
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MoveInputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_HeroMoveContext.IMC_HeroMoveContext'"));
	if (nullptr != MoveInputMappingContextRef.Object)
	{
		HeroMoveMappingContext = MoveInputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionGamePadMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_MoveGamePad.IA_MoveGamePad'"));
	if (nullptr != InputActionGamePadMoveRef.Object)
	{
		ActionGamePadMove = InputActionGamePadMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionClickMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ClickToMove.IA_ClickToMove'"));
	if (nullptr != InputActionClickMoveRef.Object)
	{
		ActionClickSetDestination = InputActionClickMoveRef.Object;
	}

	// Input-Skill
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> SkillInputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_HeroSkillContext.IMC_HeroSkillContext'"));
	if (nullptr != SkillInputMappingContextRef.Object)
	{
		HeroSkillMappingContext = SkillInputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionBaseSkillRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_BaseSkill.IA_BaseSkill'"));
	if (nullptr != InputActionBaseSkillRef.Object)
	{
		ActionBaseSkill = InputActionBaseSkillRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSkillQRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SkillQ.IA_SkillQ'"));
	if (nullptr != InputActionSkillQRef.Object)
	{
		ActionSkillQ = InputActionSkillQRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSkillWRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SkillW.IA_SkillW'"));
	if (nullptr != InputActionSkillWRef.Object)
	{
		ActionSkillW = InputActionSkillWRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSkillERef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SkillE.IA_SkillE'"));
	if (nullptr != InputActionSkillERef.Object)
	{
		ActionSkillE = InputActionSkillERef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSkillRRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SkillR.IA_SkillR'"));
	if (nullptr != InputActionSkillRRef.Object)
	{
		ActionSkillR = InputActionSkillRRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSkillFRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SkillF.IA_SkillF'"));
	if (nullptr != InputActionSkillFRef.Object)
	{
		ActionSkillF = InputActionSkillFRef.Object;
	}

	bCanAttack = true;

	// Skill Cast Component
	// SkillCastComponent = CreateDefaultSubobject<UGOSkillCastComponent>(TEXT("SkillCastComponent"));

	// Character State Init
	ActionStateBitMask = EGOPlayerActionState::None;
}

void AGOPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HeroDataAsset)
	{
		GetMesh()->SetSkeletalMesh(HeroDataAsset->SkeletalMesh);
		GetMesh()->SetAnimInstanceClass(HeroDataAsset->AnimBlueprint);
		HeroType = HeroDataAsset->HeroType;
		RoleType = HeroDataAsset->RoleType;
		AttackRange = HeroDataAsset->AttackRange;
		Archetype = HeroDataAsset->Archetype;
		// Stat->SetCharacterStat(static_cast<int32>(HeroType));
	}
}

void AGOPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SelfMove();
}

void AGOPlayerCharacter::BeginPlay()
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController)
	{
		return;
	}

	EnableInput(PlayerController);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem)
	{
		return;
	}

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(HeroMoveMappingContext, 0);
	Subsystem->AddMappingContext(HeroSkillMappingContext, 0);

	InputSubsystem = ULocalPlayer::GetSubsystem<UCommonInputSubsystem>(PlayerController->GetLocalPlayer());
	UE_LOG(LogTemp, Log, TEXT("AGOCharacterBase BeginPlay"));
}

void AGOPlayerCharacter::PossessedBy(AController* NewController)
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		GO_LOG(LogGONetwork, Log, TEXT("Owner YES1: %s"), *OwnerActor->GetName());
	}
	else
	{
		GO_LOG(LogGONetwork, Log, TEXT("Owner NO1: %s"), TEXT("No Owner"));
	}

	Super::PossessedBy(NewController);
	
	OwnerActor = GetOwner();
	if (OwnerActor)
	{
		GO_LOG(LogGONetwork, Log, TEXT("Owner YES2: %s"), *OwnerActor->GetName());
	}
	else
	{
		GO_LOG(LogGONetwork, Log, TEXT("Owner NO2: %s"), TEXT("No Owner"));
	}

	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOPlayerCharacter::OnRep_Owner()
{
	GO_LOG(LogGONetwork, Log, TEXT("%s %s"), *GetName(), TEXT("Begin"));

	Super::OnRep_Owner();

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		GO_LOG(LogGONetwork, Log, TEXT("Owner: %s"), *OwnerActor->GetName());
	}
	else
	{
		GO_LOG(LogGONetwork, Log, TEXT("Owner: %s"), TEXT("No Owner"));
	}

	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));

}

void AGOPlayerCharacter::PostNetInit()
{
	GO_LOG(LogGONetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::PostNetInit();

	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent != nullptr)
	{
		EnhancedInputComponent->BindAction(ActionGamePadMove, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::MoveGamePad);
		EnhancedInputComponent->BindAction(ActionClickSetDestination, ETriggerEvent::Started, this, &AGOPlayerCharacter::OnInputStarted);
		EnhancedInputComponent->BindAction(ActionClickSetDestination, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(ActionClickSetDestination, ETriggerEvent::Completed, this, &AGOPlayerCharacter::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(ActionClickSetDestination, ETriggerEvent::Canceled, this, &AGOPlayerCharacter::OnSetDestinationReleased);

		EnhancedInputComponent->BindAction(ActionBaseSkill, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::Attack);
		// EnhancedInputComponent->BindAction(ActionSkillQ, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSkillQ);
		EnhancedInputComponent->BindAction(ActionSkillQ, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::Attack);
		EnhancedInputComponent->BindAction(ActionSkillW, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSkillW);
		EnhancedInputComponent->BindAction(ActionSkillE, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSkillE);
		EnhancedInputComponent->BindAction(ActionSkillR, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSkillR);
		EnhancedInputComponent->BindAction(ActionSkillF, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSkillF);
	}
}

void AGOPlayerCharacter::MoveGamePad(const FInputActionValue& Value)
{
	float XAxis = Value.Get<FVector2D>().X;
	float YAxis = Value.Get<FVector2D>().Y;

	FVector Direction = FVector(YAxis, XAxis, 0.0f);

	if (InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		UE_LOG(LogTemp, Log, TEXT("Gamepad"));

		AddMovementInput(Direction, 1.0f, false);
	}
}

void AGOPlayerCharacter::OnInputStarted()
{
	if (InputSubsystem->GetCurrentInputType() == ECommonInputType::MouseAndKeyboard)
	{
		UE_LOG(LogTemp, Log, TEXT("MouseAndKeyboard"));
		GetCharacterMovement()->StopMovementImmediately();
	}
}

void AGOPlayerCharacter::OnSetDestinationTriggered()
{
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
		AddMovementInput(WorldDirection, 1.0f, false);
	}
}

void AGOPlayerCharacter::OnSetDestinationReleased()
{
	if (InputSubsystem->GetCurrentInputType() == ECommonInputType::MouseAndKeyboard && FollowTime <= ShortPressThreshold)
	{
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetActorLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();
			for (const FVector& PointLocation : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
				DrawDebugSphere(GetWorld(), PointLocation, 10.f, 5, FColor::Green, false, 2.f);
			}

			CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
			bSelfMove = true;

			for (USplineMeshComponent* SplineMeshComp : SplineMeshComponents)
			{
				if (SplineMeshComp)
				{
					SplineMeshComp->DestroyComponent();
				}
			}
			SplineMeshComponents.Empty();

			const int32 NumSplinePoints = Spline->GetNumberOfSplinePoints();
			for (int32 i = 0; i < NumSplinePoints - 1; ++i)
			{
				FVector StartPos, StartTangent, EndPos, EndTangent;
				Spline->GetLocalLocationAndTangentAtSplinePoint(i, StartPos, StartTangent);
				Spline->GetLocalLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent);

				USplineMeshComponent* SplineMeshComp = NewObject<USplineMeshComponent>(this);
				SplineMeshComp->RegisterComponentWithWorld(GetWorld());
				SplineMeshComp->SetMobility(EComponentMobility::Movable);
				SplineMeshComp->SetStaticMesh(SplineMesh);
				SplineMeshComp->SetMaterial(0, PathMaterial);
				SplineMeshComp->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
				SplineMeshComp->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
				SplineMeshComp->SetStartScale(FVector2D(SplineMeshTickness, SplineMeshTickness));
				SplineMeshComp->SetEndScale(FVector2D(SplineMeshTickness, SplineMeshTickness));
				SplineMeshComp->SetForwardAxis(ESplineMeshAxis::Z);
				SplineMeshComponents.Add(SplineMeshComp);
			}
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXImpact, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;

}

void AGOPlayerCharacter::OnSkillF()
{
	Stat->HealHp();
	UE_LOG(LogTemp, Log, TEXT("Common Skill F is triggered. Up to 20 percent of HP recovers. "));
}

void AGOPlayerCharacter::SelfMove()
{
	if (!bSelfMove) return;
	const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);
	const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
	AddMovementInput(Direction, 1.0f, false);

	const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
	if (DistanceToDestination <= SelfMoveAcceptanceRadius)
	{
		bSelfMove = false;

		for (USplineMeshComponent* SplineMeshComp : SplineMeshComponents)
		{
			if (SplineMeshComp)
			{
				SplineMeshComp->DestroyComponent();
			}
		}

		SplineMeshComponents.Empty();
	}
}

void AGOPlayerCharacter::SetupHUDWidget(UGOHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());
		InHUDWidget->UpdateManaBar(Stat->GetCurrentMana(), Stat->GetMaxMana());
		Stat->OnStatChanged.AddUObject(InHUDWidget, &UGOHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UGOHUDWidget::UpdateHpBar);
		Stat->OnManaChanged.AddUObject(InHUDWidget, &UGOHUDWidget::UpdateManaBar);
	}
}

void AGOPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGOPlayerCharacter, bCanAttack);

	DOREPLIFETIME(AGOPlayerCharacter, ActionStateBitMask);

	DOREPLIFETIME(AGOPlayerCharacter, BlownRecoveryTimer);
	DOREPLIFETIME(AGOPlayerCharacter, InvincibleTime);
	DOREPLIFETIME(AGOPlayerCharacter, InvincibleTimer);
	DOREPLIFETIME(AGOPlayerCharacter, ImpactTimer);
}

void AGOPlayerCharacter::Attack()
{
	// ProcessComboCommand();

	if (bCanAttack)
	{
		// 클라이언트에게 애니메이션 재생, 타이머 재생을 맡깁니다.
		if (!HasAuthority())
		{
			bCanAttack = false;
			// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

			GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AGOPlayerCharacter::ResetAttack, AttackTime, false);
			//FTimerHandle Handle;
			//GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			//	{
			//		bCanAttack = true;
			//		// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			//	}
			//), AttackTime, false, -1.0f);

			PlayAttackAnimation();
		}

		// 서버에게 서버시간과 함께 명령을 보냅니다.
		ServerRPCAttack(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}

void AGOPlayerCharacter::PlayAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(ComboActionMontage);
}

void AGOPlayerCharacter::AttackHitCheck()
{
	// 소유권을 가진 클라이언트가 공격 판정을 진행합니다.  
	if (IsLocallyControlled())
	{
		GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
		FHitResult OutHitResult;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

		const float DamageRange = Stat->GetTotalStat().DamageRange;
		const float DamageRadius = Stat->GetTotalStat().DamageRadius;
		const float DamageDamage = Stat->GetTotalStat().BaseDamage;
		const float DamageSpeed = Stat->GetTotalStat().DamageSpeed;

		const FVector Forward = GetActorForwardVector();
		const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
		const FVector End = Start + GetActorForwardVector() * DamageRange;

		bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_GOACTION, FCollisionShape::MakeSphere(DamageRadius), Params);

		// 공격한 시간
		float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

		// 클라이언트에서 진행 (판정에 대한 검증을 받아야 하므로 패킷 전송 필요)
		if (!HasAuthority())
		{
			if (HitDetected)
			{
				ServerRPCNotifyHit(OutHitResult, HitCheckTime);
			}
			else
			{
				ServerRPCNotifyMiss(Start, End, Forward, HitCheckTime);
			}
		}
		// 서버에서 진행 (패킷 전송 필요없이 바로 처리)
		else
		{
			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
			DrawDebugAttackRange(DebugColor, Start, End, Forward);

			if (HitDetected)
			{
				AttackHitConfirm(OutHitResult.GetActor());
			}

		}
	}
}

void AGOPlayerCharacter::AttackHitConfirm(AActor* HitActor)
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	if (HasAuthority())
	{
		const float BaseDamage = Stat->GetTotalStat().BaseDamage;
		FDamageEvent DamageEvent;
		HitActor->TakeDamage(BaseDamage, DamageEvent, GetController(), this); //?
	}
}

void AGOPlayerCharacter::DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
#if ENABLE_DRAW_DEBUG
	const float DamageRange = Stat->GetTotalStat().DamageRange;
	const float DamageRadius = Stat->GetTotalStat().DamageRadius;
	// const float AttackRadius = Stat->GetAttackRadius();

	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = DamageRange * 0.5f;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, DamageRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
#endif
}

void AGOPlayerCharacter::ClientRPCPlayAnimation_Implementation(AGOPlayerCharacter* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayAttackAnimation();
	}
}

bool AGOPlayerCharacter::ServerRPCNotifyHit_Validate(const FHitResult& HitResult, float HitChecktime)
{
	// return (HitChecktime - LastAttakStartTime) > AttackTime;
	return (HitChecktime - LastAttakStartTime) >= AcceptMinCheckTime;
}

void AGOPlayerCharacter::ServerRPCNotifyHit_Implementation(const FHitResult& HitResult, float HitChecktime)
{
	AActor* HitActor = HitResult.GetActor();
	if (::IsValid(HitActor))
	{
		const FVector HitLocation = HitResult.Location;
		const FBox HitBox = HitActor->GetComponentsBoundingBox();
		const FVector ActorBoxCenter = (HitBox.Min + HitBox.Max) * 0.5f;
		if (FVector::DistSquared(HitLocation, ActorBoxCenter) <= AcceptCheckDistance * AcceptCheckDistance)
		{
			// 대미지 전달
			AttackHitConfirm(HitActor);
		}
		else
		{
			// 기각!
			GO_LOG(LogGONetwork, Warning, TEXT("%s"), TEXT("HitTest Rejected!"));
		}

#if ENABLE_DRAW_DEBUG
		// 0.5 미터 크기로 그립니다.
		DrawDebugPoint(GetWorld(), ActorBoxCenter, 50.0f, FColor::Cyan, false, 5.0f);
		// 맞은 위치를 표시합니다.
		DrawDebugPoint(GetWorld(), HitLocation, 50.0f, FColor::Magenta, false, 5.0f);
#endif
		DrawDebugAttackRange(FColor::Green, HitResult.TraceStart, HitResult.TraceEnd, HitActor->GetActorForwardVector());
	}
}

bool AGOPlayerCharacter::ServerRPCNotifyMiss_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	return (HitCheckTime - LastAttakStartTime) > AcceptMinCheckTime;
}

void AGOPlayerCharacter::ServerRPCNotifyMiss_Implementation(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	DrawDebugAttackRange(FColor::Red, TraceStart, TraceEnd, TraceDir);
}

void AGOPlayerCharacter::OnRep_CanAttack()
{
	if (!bCanAttack)
	{
		// 공격할 수 없다면 움직이지 못합니다.
		// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		// 공격할 수 있다면 움직일 수 있습니다.
		// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AGOPlayerCharacter::ResetPlayer()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);
	}
	// Stat->SetCharacterStat(1);
	if (Stat)
	{
		Stat->ResetStat();
	}

	SetActorEnableCollision(true);

	if (HpBar) // Ensure HpBar is not null
		HpBar->SetHiddenInGame(false);

	if (ManaBar) // Ensure ManaBar is not null
		ManaBar->SetHiddenInGame(false);

	if (HasAuthority())
	{
		IGOBattleInterface* GOGameMode = GetWorld()->GetAuthGameMode<IGOBattleInterface>();
		if (GOGameMode)
		{
			FTransform NewTransform = GOGameMode->GetRandomStartTransform();
			TeleportTo(NewTransform.GetLocation(), NewTransform.GetRotation().Rotator());
		}
	}
}

void AGOPlayerCharacter::ResetAttack()
{
	bCanAttack = true;
	// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AGOPlayerCharacter::SetDead()
{
	Super::SetDead();
	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &AGOPlayerCharacter::ResetPlayer, 5.0f, false);
}

float AGOPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Stat->GetCurrentHp() <= 0.0f)
	{
		IGOBattleInterface* GOBattleMode = GetWorld()->GetAuthGameMode<IGOBattleInterface>();
		if (GOBattleMode)
		{
			GOBattleMode->OnPlayerKilled(EventInstigator, GetController(), this);
		}
	}
	return ActualDamage;
}

void AGOPlayerCharacter::SimulateStateUpdateOnServer(float DeltaTime)
{
	if (!GetCharacterMovement()->IsFalling() && IsFlashing() || !IsFlashing())
		ActionStateBitMask = ActionStateBitMask & (~EGOPlayerActionState::Flash);

	if (GetCharacterMovement()->IsMovingOnGround())
		ActionStateBitMask = ActionStateBitMask | EGOPlayerActionState::Move;
	else
		ActionStateBitMask = ActionStateBitMask & (~EGOPlayerActionState::Move);

	if (ImpactTimer > 0)
		ImpactTimer -= DeltaTime;

	if (InvincibleTimer > 0)
		InvincibleTimer -= DeltaTime;

	if (BlownRecoveryTimer > 0 && !GetCharacterMovement()->IsFalling())
		BlownRecoveryTimer -= DeltaTime;

	//if (IsImpacted() && ImpactTimer <= 0)
	//{
	//	RecoveryFromImpacted();
	//}

	//if (IsBlown() && BlownRecoveryTimer <= 0)
	//{
	//	RecoveryFromBlown();
	//}
}

bool AGOPlayerCharacter::IsExecutableOrderInOrderNotExecutableState(const FGOOrder& InOrder) const
{
	return true;
	//	(IsImpacted() && InOrder.Type == FGOOrderType::Skill1 && CurEquippedWeapon->GetSkill(0)->
	//		GetCastableOnImpacted()) ||
	//	(IsImpacted() && InOrder.Type == FGOOrderType::Skill2 && CurEquippedWeapon->GetSkill(1)->
	//		GetCastableOnImpacted()) ||
	//	(IsBlown() && InOrder.Type == FGOOrderType::Skill1 && CurEquippedWeapon->GetSkill(0)->GetCastableOnBlown()) ||
	//	(IsBlown() && InOrder.Type == FGOOrderType::Skill2 && CurEquippedWeapon->GetSkill(1)->GetCastableOnBlown());
}

bool AGOPlayerCharacter::ServerRPCAttack_Validate(float AttackStartTime)
{
	if (LastAttakStartTime == 0.0f)
	{
		return true;
	}

	return (AttackStartTime - LastAttakStartTime) > (AttackTime - 0.4f);
}

// (2) 서버가 실행하는 함수입니다.
void AGOPlayerCharacter::ServerRPCAttack_Implementation(float AttackStartTime)
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	bCanAttack = false;
	Stat->UseSkill(Stat->GetTotalStat().BaseDamage); // TODO: ManaCost

	// 프로퍼티 OnRep 함수를 명시적 호출합니다.
	OnRep_CanAttack();

	AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;
	GO_LOG(LogGONetwork, Log, TEXT("LagTime: %f"), AttackTimeDifference);
	AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.0f, AttackTime - 0.01f);

	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AGOPlayerCharacter::ResetAttack, AttackTime - AttackTimeDifference, false);

	//FTimerHandle Handle;
	//GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
	//	{
	//		bCanAttack = true;
	//		OnRep_CanAttack();
	//	}
	//), AttackTime - AttackTimeDifference, false, -1.0f);

	LastAttakStartTime = AttackStartTime;
	PlayAttackAnimation();
	
	// MulticastRPCAttack();
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				// 이 조건문을 통과한 컨트롤러는 simulated proxy로 캐릭터를 재생하는 다른 플레이어 컨트롤러입니다.
				AGOPlayerCharacter* OtherPlayer = Cast<AGOPlayerCharacter>(PlayerController->GetPawn());
				if (OtherPlayer)
				{
					OtherPlayer->ClientRPCPlayAnimation(this);
				}
			}
		}
	}
}

// (3) Multicast 는 게임과 무관한 효과를 재생하는 것이 좋습니다.
void AGOPlayerCharacter::MulticastRPCAttack_Implementation()
{
	if (!IsLocallyControlled())
	{
		// 현재 클라이언트는 이미 모션을 재생했으므로
		// 다른 클라이언트의 프록시로써 동작하는 캐릭터에 대해서만 모션을 재생시킵니다.
		PlayAttackAnimation();
	}
}
