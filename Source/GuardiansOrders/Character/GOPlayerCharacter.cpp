// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GOPlayerCharacter.h"
#include "UI/GOWidgetComponent.h" 
#include "UI/GOHpBarWidget.h"
#include "UI/GOManaBarWidget.h"
#include "UI/GOHUDWidget.h"
#include "UI/SkillWidget/GOSkillSetBarWidget.h"
#include "UI/SpellWidget/GOSpellSetBarWidget.h"
#include "UI/GOHeroInfoWidget.h"
#include "CharacterStat/GOCharacterStatComponent.h"
#include "Skill/GOSkillCastComponent.h"
#include "Skill/GOSpellCastComponent.h"
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
#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include "Components/DecalComponent.h"
#include "Player/GOPlayerController.h"
#include "GOCharacterMovementComponent.h"
#include "Share/EGOSkill.h"
#include "Share/EGOTeam.h"
#include <Game/GOBattleGameMode.h>
#include "GameData/GOSkillData.h"
#include "Game/GOPlayerState.h"
#include "UI/GOStatsBarWidget.h"
#include "UI/GOGrindingStoneWidget.h"
#include "UI/GOHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Engine/Texture2D.h"
#include "NiagaraComponent.h"

AGOPlayerCharacter::AGOPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGOCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)),
	bIsDecalVisible(false)
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
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

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSpellDRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SpellD_Flash.IA_SpellD_Flash'"));
	if (nullptr != InputActionSpellDRef.Object)
	{
		ActionSpellD = InputActionSpellDRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSpellFRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SpellF_Heal.IA_SpellF_Heal'"));
	if (nullptr != InputActionSpellFRef.Object)
	{
		ActionSpellF = InputActionSpellFRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSpellGRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SpellG_Ghost.IA_SpellG_Ghost'"));
	if (nullptr != InputActionSpellGRef.Object)
	{
		ActionSpellG = InputActionSpellGRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDecalARef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_MaxBasicAttackRange.IA_MaxBasicAttackRange'"));
	if (nullptr != InputActionDecalARef.Object)
	{
		ActionShowMaxBasicAttackRange = InputActionDecalARef.Object;
	}

	bCanAttack = true;
}

void AGOPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AGOPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead == false)
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

	BindWidgetEvents();
	// StatComponent와 HUDWidget 연결
	if (AGOPlayerController* GOPlayerController = Cast<AGOPlayerController>(GetController()))
	{
		GOPlayerController->GOHUDWidget->BindToStatComponent(Stat);
	}
}

void AGOPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AGOPlayerCharacter::OnRep_Owner()
{
	Super::OnRep_Owner();
}

void AGOPlayerCharacter::PostNetInit()
{
	Super::PostNetInit();
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

		EnhancedInputComponent->BindAction(ActionBaseSkill, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnBaseSkill);
		EnhancedInputComponent->BindAction(ActionSkillQ, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSkillQ);
		EnhancedInputComponent->BindAction(ActionSkillW, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSkillW);
		EnhancedInputComponent->BindAction(ActionSkillE, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSkillE);
		EnhancedInputComponent->BindAction(ActionSkillR, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSkillR);

		EnhancedInputComponent->BindAction(ActionSpellD, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSpellD);
		EnhancedInputComponent->BindAction(ActionSpellF, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSpellF);
		EnhancedInputComponent->BindAction(ActionSpellG, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSpellG);

		EnhancedInputComponent->BindAction(ActionShowMaxBasicAttackRange, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnShowMaxBasicAttackRange);
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
	if (bIsDead == true || bIsStunned == true) return;
	if (AGOPlayerController* GOPlayerController = Cast<AGOPlayerController>(GetController()))
	{
		if (!GOPlayerController->CheckMatchState())
		{
			return;
		}
	}

	if (InputSubsystem->GetCurrentInputType() == ECommonInputType::MouseAndKeyboard)
	{
		UE_LOG(LogTemp, Log, TEXT("MouseAndKeyboard"));
		GetCharacterMovement()->StopMovementImmediately();
	}
}

void AGOPlayerCharacter::OnSetDestinationTriggered()
{
	if (bIsDead == true || bIsStunned == true) return;
	if (AGOPlayerController* GOPlayerController = Cast<AGOPlayerController>(GetController()))
	{
		if (!GOPlayerController->CheckMatchState())
		{
			return;
		}
	}

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
	if (bIsDead == true || bIsStunned == true) return;
	if (AGOPlayerController* GOPlayerController = Cast<AGOPlayerController>(GetController()))
	{
		if (!GOPlayerController->CheckMatchState())
		{
			return;
		}
	}

	if (InputSubsystem->GetCurrentInputType() == ECommonInputType::MouseAndKeyboard && FollowTime <= ShortPressThreshold)
	{
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetActorLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();
			for (const FVector& PointLocation : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
				//DrawDebugSphere(GetWorld(), PointLocation, 10.f, 5, FColor::Green, false, 2.f);
			}
			if (NavPath->PathPoints.Num() > 0)
			{
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bSelfMove = true;
			}

			//for (USplineMeshComponent* SplineMeshComp : SplineMeshComponents)
			//{
			//	if (SplineMeshComp)
			//	{
			//		SplineMeshComp->DestroyComponent();
			//	}
			//}
			//SplineMeshComponents.Empty();

			//const int32 NumSplinePoints = Spline->GetNumberOfSplinePoints();
			//for (int32 i = 0; i < NumSplinePoints - 1; ++i)
			//{
			//	FVector StartPos, StartTangent, EndPos, EndTangent;
			//	Spline->GetLocalLocationAndTangentAtSplinePoint(i, StartPos, StartTangent);
			//	Spline->GetLocalLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent);

			//	USplineMeshComponent* SplineMeshComp = NewObject<USplineMeshComponent>(this);
			//	SplineMeshComp->RegisterComponentWithWorld(GetWorld());
			//	SplineMeshComp->SetMobility(EComponentMobility::Movable);
			//	SplineMeshComp->SetStaticMesh(SplineMesh);
			//	SplineMeshComp->SetMaterial(0, PathMaterial);
			//	SplineMeshComp->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
			//	SplineMeshComp->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
			//	SplineMeshComp->SetStartScale(FVector2D(SplineMeshTickness, SplineMeshTickness));
			//	SplineMeshComp->SetEndScale(FVector2D(SplineMeshTickness, SplineMeshTickness));
			//	SplineMeshComp->SetForwardAxis(ESplineMeshAxis::Z);
			//	SplineMeshComponents.Add(SplineMeshComp);
			//}
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXImpact, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
	else
	{
		return;
	}
	FollowTime = 0.f;

}

void AGOPlayerCharacter::OnBaseSkill()
{
	if (bIsDead == true || bIsStunned == true) return;
	SkillCastComponent->OnStartCast(
		FHeroSkillKey(CharacterData.HeroType, ECharacterSkills::BaseSkill));
}

void AGOPlayerCharacter::OnSkillQ()
{
	if (bIsDead == true || bIsStunned == true) return;
	SkillCastComponent->OnStartCast(
		FHeroSkillKey(CharacterData.HeroType, ECharacterSkills::Skill01));
}

void AGOPlayerCharacter::OnSkillW()
{
	if (bIsDead == true || bIsStunned == true) return;
	SkillCastComponent->OnStartCast(
		FHeroSkillKey(CharacterData.HeroType, ECharacterSkills::Skill02));

}

void AGOPlayerCharacter::OnSkillE()
{
	if (bIsDead == true || bIsStunned == true) return;
	SkillCastComponent->OnStartCast(
		FHeroSkillKey(CharacterData.HeroType, ECharacterSkills::Skill03));
}

void AGOPlayerCharacter::OnSkillR()
{
	if (bIsDead == true || bIsStunned == true) return;
	SkillCastComponent->OnStartCast(
		FHeroSkillKey(CharacterData.HeroType, ECharacterSkills::UltimateSkill));
}

void AGOPlayerCharacter::OnSpellD()
{
	if (bIsDead == true || bIsStunned == true) return;
	UGOCharacterMovementComponent* GOMovement = Cast<UGOCharacterMovementComponent>(GetCharacterMovement());
	if (GOMovement)
	{
		GOMovement->SetFlashSpellCommand();
	}
}

void AGOPlayerCharacter::OnSpellF()
{
	if (bIsDead == true || bIsStunned == true) return;
	if (SpellCastComponent->OnStartCast(
		FHeroSpellKey(CharacterData.HeroType, ECharacterSpells::Spell02)) == true)
	{
		if (HasAuthority())
		{
			Stat->HealHp(0.f);
		}
		else
		{
			Stat->ServerHealHp(0.f);
		}
	};
}

// Ghost Spell
void AGOPlayerCharacter::OnSpellG()
{
	if (bIsDead == true || bIsStunned == true) return;
	if (SpellCastComponent->OnStartCast(
		FHeroSpellKey(CharacterData.HeroType, ECharacterSpells::Spell03)) == true)
	{
		UGOCharacterMovementComponent* GOMovement = Cast<UGOCharacterMovementComponent>(GetCharacterMovement());
		if (GOMovement)
		{
			GOMovement->SetGhostSpellCommand();
		}
	}
}

void AGOPlayerCharacter::OnShowMaxBasicAttackRange()
{
	float MaxBasicAttackRange = Stat->GetMaxBasicAttackRange();

	if (!BasicAttackRangeDecal)
	{
		BasicAttackRangeDecal = NewObject<UDecalComponent>(this, UDecalComponent::StaticClass());
		if (BasicAttackRangeDecal)
		{
			BasicAttackRangeDecal->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			BasicAttackRangeDecal->SetWorldRotation(FRotator(90.0f, 0.0f, 0.0f));
			BasicAttackRangeDecal->SetRelativeLocation(FVector(0.f, 0.f, -150.f));
			// BasicAttackRangeDecal->SetRelativeScale3D(FVector(1, 1, 1));
			BasicAttackRangeDecal->SetDecalMaterial(LoadObject<UMaterialInterface>(nullptr, TEXT("Engine.Material'/Game/Material/M_MaxBasicAttackRangeDecal.M_MaxBasicAttackRangeDecal'")));
			BasicAttackRangeDecal->RegisterComponent();
			BasicAttackRangeDecal->SetVisibility(false);
		}
	}

	float MinScale = 1.f;
	float ScaleFactor = 200.0f; // 사거리를 스케일로 변환하는 기준 값
	float DecalScale = FMath::Max(MinScale, MaxBasicAttackRange / ScaleFactor);
	BasicAttackRangeDecal->SetRelativeScale3D(FVector(DecalScale, DecalScale, DecalScale));

	bIsDecalVisible = !bIsDecalVisible;
	BasicAttackRangeDecal->SetVisibility(bIsDecalVisible);
}

void AGOPlayerCharacter::SelfMove()
{
	if (bIsDead == true || bIsStunned == true) return;
	if (!bSelfMove) return;
	const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);
	const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
	AddMovementInput(Direction, 1.0f, false);

	const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
	if (DistanceToDestination <= SelfMoveAcceptanceRadius)
	{
		bSelfMove = false;

		//for (USplineMeshComponent* SplineMeshComp : SplineMeshComponents)
		//{
		//	if (SplineMeshComp)
		//	{
		//		SplineMeshComp->DestroyComponent();
		//	}
		//}

		//SplineMeshComponents.Empty();
	}
}

void AGOPlayerCharacter::SetupHUDWidget(UGOHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		// Stats
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());
		InHUDWidget->UpdateManaBar(Stat->GetCurrentMana(), Stat->GetMaxMana());
		Stat->OnStatChanged.AddUObject(InHUDWidget, &UGOHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UGOHUDWidget::UpdateHpBar);
		Stat->OnManaChanged.AddUObject(InHUDWidget, &UGOHUDWidget::UpdateManaBar);
		// Skill
		InHUDWidget->GetSkillSetBar()->InitializeSkillSlots(CharacterSkillSet);
		// HeroInfo
		InHUDWidget->GetHeroInfo()->BindHeroInfo(CharacterData);
		// Spell
		InHUDWidget->GetSpellSetBar()->InitializeSpellSlots(CharacterSpellSet);
	}
}

void AGOPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGOPlayerCharacter, bCanAttack);
	DOREPLIFETIME(AGOPlayerCharacter, NetRotation);
}

void AGOPlayerCharacter::SkillAttackHitCheck()
{
	TObjectPtr<UGOSkillBase> CurrentSkill = GetSkillCastComponent()->GetCurrentSkill();

	UGOGameSubsystem* GameSubsystem = GetGameInstance()->GetSubsystem<UGOGameSubsystem>();
	FHeroSkillKey Key;

	// 소유권을 가진 클라이언트가 공격 판정을 진행합니다.  
	if (IsLocallyControlled())
	{
		if (GameSubsystem)
		{
			Key = GameSubsystem->GetKeyBySkillObject(CurrentSkill);

			ServerRPCActivateSkillWithParticles(Key);
		}

		bool HitDetected = CurrentSkill->GetHitDetected();
		ESkillAffectType SkillAffectType = CurrentSkill->GetSkillAffectType();

		FGOOutHitCollisionStructure& OutHitCollisionStructure = CurrentSkill->GetOutHitCollisionStructure();
		FGOOutHitCollisionStructure SkillHitCollisionStructure = OutHitCollisionStructure; // 추가

		FHitResult OutHitResult = OutHitCollisionStructure.OutHitResult;
		TArray<FHitResult> OutHitResults = OutHitCollisionStructure.OutHitResults;
		TArray<FOverlapResult> OutOverlaps = OutHitCollisionStructure.OutOverlaps;

		ESkillCollisionType CurrentSkillCollisionType = CurrentSkill->GetSkillCollisionType();

		float SkillAffectAmount;

		// 연마석?
		switch (SkillAffectType) {
		case ESkillAffectType::Damage:
			SkillAffectAmount = CurrentSkill->GetTotalSkillStat().DamageMultiplier * Stat->GetTotalStat().BaseDamage;
			break;
		case ESkillAffectType::Defense:
			SkillAffectAmount = CurrentSkill->GetTotalSkillStat().DamageMultiplier * Stat->GetTotalStat().BaseDefense;
			break;
		case ESkillAffectType::Heal:
			SkillAffectAmount = CurrentSkill->GetTotalSkillStat().DamageMultiplier * Stat->GetTotalStat().BaseHeal;
			break;
		default:
			break;
		}

		// 공격한 시간
		float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

		// 공격

		// 클라이언트에서 진행 (판정에 대한 검증을 받아야 하므로 패킷 전송 필요)
		if (!HasAuthority())
		{
			PlayEffectParticleAnimByKey(Key);

			if (HitDetected)
			{
				AActor* HitActor = nullptr;
				switch (CurrentSkillCollisionType)
				{
				case ESkillCollisionType::LineTraceSingle:
					HitActor = OutHitCollisionStructure.OutHitResult.GetActor();
					if (IsValid(HitActor)) {
						ServerRPCNotifySkillHitTest(OutHitResult, SkillAffectAmount, SkillAffectType, Key);
					}
					break;

				case ESkillCollisionType::LineTraceMulti:
					for (const FHitResult& HitResult : OutHitCollisionStructure.OutHitResults) {
						HitActor = HitResult.GetActor();
						if (IsValid(HitActor)) {
							ServerRPCNotifySkillHitResults(OutHitResults, SkillAffectAmount, SkillAffectType, Key);
						}
					}
					break;

				case ESkillCollisionType::SweepSingle:
					HitActor = OutHitCollisionStructure.OutHitResult.GetActor();
					if (HitActor != nullptr) {
						ServerRPCNotifySkillHitTest(OutHitResult, SkillAffectAmount, SkillAffectType, Key);
					}
					break;

				case ESkillCollisionType::SweepMulti:
					ServerRPCNotifySkillHitResults(OutHitResults, SkillAffectAmount, SkillAffectType, Key);
					break;
				case ESkillCollisionType::OverlapMulti:
					ServerRPCNotifySkillHitOverlapResult(OutOverlaps, SkillAffectAmount, SkillAffectType, Key);
					break;

				default:
					break;
				}
			}
			else
			{
				ServerRPCNotifySkillMiss(HitCheckTime);
			}
			if (HitCameraShakeClass)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass, CurrentSkill->GetTotalSkillData().CameraShakeIntensity);
			}
		}
		else
		{
			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
			if (HitDetected)
			{
				AActor* HitActor = nullptr;
				switch (CurrentSkillCollisionType)
				{
				case ESkillCollisionType::LineTraceSingle:
					HitActor = SkillHitCollisionStructure.OutHitResult.GetActor();
					if (IsValid(HitActor)) {
						SkillHitConfirm(HitActor, SkillAffectAmount, SkillAffectType);
					}
					break;

				case ESkillCollisionType::LineTraceMulti:
					for (const FHitResult& HitResult : SkillHitCollisionStructure.OutHitResults) {
						HitActor = HitResult.GetActor();
						if (IsValid(HitActor)) {
							SkillHitConfirm(HitActor, SkillAffectAmount, SkillAffectType);
						}
					}
					break;

				case ESkillCollisionType::SweepSingle:
					HitActor = SkillHitCollisionStructure.OutHitResult.GetActor();
					if (HitActor != nullptr) {
						SkillHitConfirm(OutHitResult.GetActor(), SkillAffectAmount, SkillAffectType);
					}
					break;

				case ESkillCollisionType::SweepMulti:
					for (const FHitResult& HitResult : SkillHitCollisionStructure.OutHitResults) {
						HitActor = HitResult.GetActor();
						if (IsValid(HitActor)) {
							SkillHitConfirm(HitActor, SkillAffectAmount, SkillAffectType);
						}
					}
					break;

				case ESkillCollisionType::OverlapMulti:
					for (const FOverlapResult& OverlapResult : SkillHitCollisionStructure.OutOverlaps) {
						HitActor = OverlapResult.GetActor();
						if (IsValid(HitActor)) {
							SkillHitConfirm(HitActor, SkillAffectAmount, SkillAffectType);
						}
					}
					break;

				default:
					break;
				}
			}
			if (HitCameraShakeClass)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass, CurrentSkill->GetTotalSkillData().CameraShakeIntensity);
			}
		}

		// 초기화
		//OutHitCollisionStructure.OutHitResult = FHitResult();
		//OutHitCollisionStructure.OutHitResults.Empty();
		//OutHitCollisionStructure.OutOverlaps.Empty();
		OutHitCollisionStructure.Clear();
	}
}

void AGOPlayerCharacter::SkillHitConfirm(AActor* HitActor, float SkillAffectAmount, ESkillAffectType SkillAffectType)
{
	if (HasAuthority())
	{
		FDamageEvent DamageEvent;
		AGOPlayerCharacter* TargetCharacter = Cast<AGOPlayerCharacter>(HitActor);
		AGOPlayerState* TargetPlayerState = Cast<AGOPlayerState>(TargetCharacter->GetPlayerState());
		AGOPlayerState* MyPlayerState = Cast<AGOPlayerState>(GetPlayerState());

		switch (SkillAffectType) {
		case ESkillAffectType::Damage:
			HitActor->TakeDamage(SkillAffectAmount, DamageEvent, GetController(), this);
			break;

		case ESkillAffectType::Defense:
			// TODO
			UE_LOG(LogTemp, Warning, TEXT("AGOPlayerCharacter::HandleDefenseSkill: %f"), SkillAffectAmount);
			break;

		case ESkillAffectType::Heal:
			if (TargetPlayerState->Team != MyPlayerState->Team) return;

			if (TargetCharacter->HasAuthority())
			{
				TargetCharacter->Stat->HealHp(SkillAffectAmount);
			}
			else
			{
				TargetCharacter->Stat->ServerHealHp(SkillAffectAmount);
			}
			MulticastSpawnHealEffect(TargetCharacter);
			break;
		default:
			break;
		}
	}
}

void AGOPlayerCharacter::ClientRPCPlaySkillAnimation_Implementation(AGOPlayerCharacter* CharacterToPlay, UGOSkillBase* CurrentSkill)
{
	if (CharacterToPlay)
	{
		if (CurrentSkill == nullptr)
		{
			return;
		}

		CharacterToPlay->PlaySkillAnim(CurrentSkill);
	}
}

// 새로 만든: 구조체
void AGOPlayerCharacter::ClientRPCActivateSkill_Implementation(AGOPlayerCharacter* CharacterToPlay, FHeroSkillKey Key)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlaySkillAnimByKey(Key);
	}
}

bool AGOPlayerCharacter::ServerRPCNotifySkillHitTest_Validate(const FHitResult& HitResult, float DamageAmount, ESkillAffectType SkillAffectType, FHeroSkillKey Key)
{
	return true;
}

void AGOPlayerCharacter::ServerRPCNotifySkillHitTest_Implementation(const FHitResult& HitResult, float DamageAmount, ESkillAffectType SkillAffectType, FHeroSkillKey Key)
{
	AActor* HitActor = HitResult.GetActor();
	if (::IsValid(HitActor))
	{
		const FVector HitLocation = HitResult.Location;
		const FBox HitBox = HitActor->GetComponentsBoundingBox();
		const FVector ActorBoxCenter = (HitBox.Min + HitBox.Max) * 0.5f;
		SkillHitConfirm(HitActor, DamageAmount, SkillAffectType);
		if (FVector::DistSquared(HitLocation, ActorBoxCenter) <= AcceptCheckDistance * AcceptCheckDistance)
		{
			// 대미지 전달
			//AttackHitConfirm(HitActor);
			// AttackSkillHitConfirm(HitActor, DamageAmount);
		}
		else
		{
			// 기각!
			GO_LOG(LogGONetwork, Warning, TEXT("%s"), TEXT("[HIT!!] HitTest Rejected!"));
		}
	}
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));

}

bool AGOPlayerCharacter::ServerRPCNotifySkillMiss_Validate(float HitCheckTime)
{
	return true;
}

void AGOPlayerCharacter::ServerRPCNotifySkillMiss_Implementation(float HitCheckTime)
{
}

void AGOPlayerCharacter::ServerRPCNotifySkillHitResults_Implementation(const TArray<FHitResult>& HitResult, float DamageAmount, ESkillAffectType SkillAffectType, FHeroSkillKey Key)
{
	AActor* HitActor = nullptr;

	for (const FHitResult& HitResult : HitResult) {
		HitActor = HitResult.GetActor();
		if (IsValid(HitActor)) {
			SkillHitConfirm(HitActor, DamageAmount, SkillAffectType);
		}
	}
}

void AGOPlayerCharacter::ServerRPCNotifySkillHitOverlapResult_Implementation(const TArray<FOverlapResult>& FOverlapResults, float DamageAmount, ESkillAffectType SkillAffectType, FHeroSkillKey Key)
{
	AActor* HitActor = nullptr;

	for (const FOverlapResult& OverlapResult : FOverlapResults) {
		HitActor = OverlapResult.GetActor();
		if (IsValid(HitActor)) {

			SkillHitConfirm(HitActor, DamageAmount, SkillAffectType);
		}
	}
}

void AGOPlayerCharacter::OnRep_CanAttack()
{
	if (!bCanAttack)
	{
		// 공격할 수 없다면 움직이지 못합니다.
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		// 공격할 수 있다면 움직일 수 있습니다.
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AGOPlayerCharacter::ResetPlayer()
{
	bIsDead = false; // Add this line

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);
	}

	if (Stat)
	{
		Stat->ResetStat();
	}

	SetActorEnableCollision(true);

	if (HpBar) // Ensure HpBar is not null
		HpBar->SetHiddenInGame(false);

	if (ManaBar) // Ensure ManaBar is not null
		ManaBar->SetHiddenInGame(false);

	if (StatsBar)
		StatsBar->SetHiddenInGame(false);

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
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AGOPlayerCharacter::SetDead()
{
	Super::SetDead();
	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &AGOPlayerCharacter::ResetPlayer, 5.0f, false);
}

void AGOPlayerCharacter::SetStunned()
{
	Super::SetStunned();
	MulticastStunnedAnimation();
	Knockback(KnockbackDirection);
	// 공격한 사람의 foward vector로 밀려나기
	// 1초 후 stunned 상태에서 풀려나기
}

void AGOPlayerCharacter::EndStunned()
{
	bIsStunned = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AGOPlayerCharacter::Knockback(const FVector& Direction)
{
	LaunchCharacter(Direction * 500.0f, true, true);
}

// EventInstigator: Attacker
float AGOPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead == true) return 0;

	GOBattleGameMode = GOBattleGameMode == nullptr ? GetWorld()->GetAuthGameMode<AGOBattleGameMode>() : GOBattleGameMode;
	if (GOBattleGameMode == nullptr) return 0.0f;

	DamageAmount = GOBattleGameMode->CalculateDamage(EventInstigator, Controller, DamageAmount); // EventInstigator: Attacker. Controller: Victim

	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Stat->GetCurrentHp() <= 0.0f)
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
	else
	{
		if (!bIsStunned)
		{
			KnockbackDirection = -DamageCauser->GetActorForwardVector();
			SetStunned();
			GetWorldTimerManager().SetTimer(StunnedTimerHandle, this, &AGOPlayerCharacter::EndStunned, 1.0f, false);
		}
	}
	return ActualDamage;
}

void AGOPlayerCharacter::SetTeamColor(ETeamType TeamtoSet)
{
	if (TeamtoSet == ETeamType::ET_BlueTeam)
	{
	}
	else if (TeamtoSet == ETeamType::ET_RedTeam)
	{
	}
}

// 새로 구조체
bool AGOPlayerCharacter::ServerRPCActivateSkill_Validate(float AttackStartTime, FHeroSkillKey Key)
{
	return true;
}

// 새로 구조체
void AGOPlayerCharacter::ServerRPCActivateSkill_Implementation(float AttackStartTime, FHeroSkillKey Key)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	if (Key.SkillType != ECharacterSkills::BaseSkill)
	{
		// ManaCost
		if (Stat->GetCurrentMana() < GOGameInstance->GetSkillByHeroSkillKey(Key)->GetTotalSkillStat().ManaCost)
		{
			return;
		}
		Stat->UseSkill(GOGameInstance->GetSkillByHeroSkillKey(Key)->GetTotalSkillStat().ManaCost);
	}

	AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;
	AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.0f, AttackTime - 0.01f);

	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AGOPlayerCharacter::ResetAttack, AttackTime - AttackTimeDifference, false);

	LastAttakStartTime = AttackStartTime;

	PlaySkillAnimByKey(Key);

	// 스킬 이펙트 효과
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
					OtherPlayer->ClientRPCActivateSkill(this, Key); // 여기가 안됨
				}
			}
		}
	}
}

// 스킬 이펙트 효과 MulticastRPCActivateSkil
void AGOPlayerCharacter::MulticastRPCActivateSkill_Implementation(FHeroSkillKey Key)
{
	if (!IsLocallyControlled())
	{
		PlayEffectParticleAnimByKey(Key);
	}
}

// ======== IGOPlaySkillAnimInterface ========
void AGOPlayerCharacter::PlaySkillAnim(UGOSkillBase* CurrentSkill)
{
	if (!CurrentSkill)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->Montage_Play(CurrentSkill->GetTotalSkillData().SkillAnim);
}

// 새로 구조체
void AGOPlayerCharacter::PlaySkillAnimByKey(FHeroSkillKey Key)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	UGOSkillBase* CurrentSkill = GOGameInstance->GetSkillByHeroSkillKey(Key);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->Montage_Play(CurrentSkill->GetTotalSkillData().SkillAnim);
}

// 새로 구조체
void AGOPlayerCharacter::ActivateSkillByKey(FHeroSkillKey Key)
{
	if (!HasAuthority())
	{
		PlaySkillAnimByKey(Key);
	}
	ServerRPCActivateSkill(GetWorld()->GetGameState()->GetServerWorldTimeSeconds(), Key);
}

void AGOPlayerCharacter::OnRep_Rotation()
{
	SetActorRotation(NetRotation);
}

bool AGOPlayerCharacter::ServerSetRotation_Validate(FRotator NewRotation)
{
	return true;
}

void AGOPlayerCharacter::ServerSetRotation_Implementation(FRotator NewRotation)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		NetRotation = NewRotation;
		OnRep_Rotation(); // 동기화를 위해 직접 호출
	}
}

// ======== IGOSpellFlashInterface ========
void AGOPlayerCharacter::ActivateSpellFlash()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FHitResult HitResult;
	FVector CursorLocation;
	if (PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		CursorLocation = HitResult.Location;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector Direction = (CursorLocation - CurrentLocation).GetSafeNormal();
	FRotator NewRotation = Direction.Rotation();

	// 현재 위치와 커서 위치의 거리
	float MaxDistance = CharacterSpellSet->GetSpell(ECharacterSpells::Spell01)->GetTotalSpellStat().MoveSpeedMultiplier;
	FVector TargetLocation;
	if (FVector::Dist(CurrentLocation, CursorLocation) <= MaxDistance)
	{
		TargetLocation = CursorLocation;
	}
	else
	{
		// 최대 이동 거리를 초과할 경우, 해당 방향으로 최대 거리만큼만 이동
		TargetLocation = CurrentLocation + Direction * MaxDistance;
	}

	// 캐릭터를 회전시킨 후 순간이동
	SetActorRotation(NewRotation);
	TeleportTo(TargetLocation, NewRotation, false, true);
}

// ======== IGOPlaySkillEffectInterface ========
void AGOPlayerCharacter::PlayEffectParticleAnimByKey(FHeroSkillKey Key)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	UGOSkillBase* CurrentSkill = GOGameInstance->GetSkillByHeroSkillKey(Key);

	UParticleSystem* DefenseSkillEffect = CurrentSkill->GetTotalSkillData().SkillEffect;
	if (DefenseSkillEffect)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefenseSkillEffect, SpawnLocation, SpawnRotation);
	}
}

// ======== IGOApplySkillInterface ========
void AGOPlayerCharacter::ApplySkillEffect(AActor* DamagedActor, float Damage, AActor* DamageCauser)
{
	if (!DamagedActor || !DamageCauser) return;  // Check for null pointers

	TakeDamage(Damage, FDamageEvent(), DamageCauser->GetInstigatorController(), DamageCauser);
}


void AGOPlayerCharacter::HighlightActor()
{
	Super::HighlightActor();
}

void AGOPlayerCharacter::UnHighlightActor()
{
	Super::UnHighlightActor();
}

bool AGOPlayerCharacter::GetIsDead()
{
	return Super::GetIsDead();
}

bool AGOPlayerCharacter::GetIsStunned()
{
	return Super::GetIsStunned();
}

void AGOPlayerCharacter::ServerRPCActivateSkillWithParticles_Implementation(FHeroSkillKey Key)
{
	PlayEffectParticleAnimByKey(Key);
	MulticastRPCActivateSkillWithParticles(Key);
}

void AGOPlayerCharacter::MulticastRPCActivateSkillWithParticles_Implementation(FHeroSkillKey Key)
{
	if (!IsLocallyControlled())
	{
		PlayEffectParticleAnimByKey(Key);
	}
}

void AGOPlayerCharacter::MulticastStunnedAnimation_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(StunnedMontage, 1.0f);
}

ETeamType AGOPlayerCharacter::GetTeamType()
{
	GOPlayerState = GOPlayerState == nullptr ? GetPlayerState<AGOPlayerState>() : GOPlayerState;
	if (GOPlayerState == nullptr) return ETeamType::ET_NoTeam;
	return GOPlayerState->GetTeamType();
}

// 연마석
void AGOPlayerCharacter::ServerCheckForGrindingStone_Implementation()
{
	CheckForGrindingStone();
}

bool AGOPlayerCharacter::ServerCheckForGrindingStone_Validate()
{
	return true;
}

void AGOPlayerCharacter::ClientSetGrindingStoneVisible_Implementation()
{
	AGOPlayerState* PS = GetPlayerState<AGOPlayerState>();
	if (PS && PS->bHasGrindingStone)
	{
		PS->SetGrindingStone(true);
	}
}

void AGOPlayerCharacter::HandlePlayerKilled(AController* KillerController, AGOPlayerState* VictimPlayerState)
{
	if (KillerController)
	{
		AGOPlayerState* KillerPlayerState = Cast<AGOPlayerState>(KillerController->PlayerState);
		if (KillerPlayerState && VictimPlayerState)
		{
			KillerPlayerState->AddKilledEnemyPlayer(VictimPlayerState->GetPlayerId());
			KillerPlayerState->CheckForGrindingStone();
		}
	}
}

void AGOPlayerCharacter::CheckForGrindingStone()
{
	AGOPlayerState* PS = GetPlayerState<AGOPlayerState>();
	if (PS)
	{
		PS->CheckForGrindingStone();
	}
}

void AGOPlayerCharacter::AttemptStatIncrease()
{
	ServerAttemptStatIncrease();
}

void AGOPlayerCharacter::BindWidgetEvents()
{
	if (AGOPlayerController* GOPlayerController = Cast<AGOPlayerController>(GetController()))
	{
		if (UGOGrindingStoneWidget* GrindingStoneWidget = Cast<UGOGrindingStoneWidget>(GOPlayerController->GOHUDWidget->GrindingStoneWidget))
		{
			GrindingStoneWidget->OnYesButtonClicked.AddUObject(this, &AGOPlayerCharacter::AttemptStatIncrease);
		}
	}

}

void AGOPlayerCharacter::ServerAttemptStatIncrease_Implementation()
{
	// 확률 및 스탯 증가 로직
	TArray<float> Chances = { 0.99f, 0.70f, 0.50f, 0.30f, 0.10f };
	TArray<float> StatIncreases = { 5, 10, 25, 40, 50 };
	if (AttemptCount >= Chances.Num())
	{
		AttemptCount = Chances.Num() - 1; // 최대 값으로 제한
	}
	float Chance = Chances[AttemptCount];
	float StatIncreaseAmount = StatIncreases[AttemptCount];
	bool bSuccess = FMath::FRand() <= Chance;

	if (bSuccess)
	{
		float OldBaseDamage = Stat->GetTotalStat().BaseDamage;

		if (Stat)
		{
			Stat->IncreaseBaseDamage(StatIncreaseAmount);
			FVector CharacterLocation = GetActorLocation();
			MulticastSpawnGrindingNiagaraEffect(CharacterLocation);
		}
		// Assume Stat is a struct with a BaseDamage property.
		float NewBaseDamage = Stat->GetTotalStat().BaseDamage;
	}
}

bool AGOPlayerCharacter::ServerAttemptStatIncrease_Validate()
{
	return true;
}
void AGOPlayerCharacter::MulticastSpawnGrindingNiagaraEffect_Implementation(FVector Location)
{
	SpawnGrindingNiagaraEffect(Location);
}

void AGOPlayerCharacter::SpawnGrindingNiagaraEffect(FVector Location)
{
	if (FXGrindingImpact)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FXGrindingImpact, Location, FRotator::ZeroRotator, FVector(1.f), true, true, ENCPoolMethod::None, true);
	}
}

void AGOPlayerCharacter::MulticastSpawnHealEffect_Implementation(AGOPlayerCharacter* TargetCharacter)
{
	if (TargetCharacter && HealEffect) // Ensure HealEffect is a valid UNiagaraSystem*
	{
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			HealEffect,
			TargetCharacter->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true);

		if (NiagaraComponent)
		{
			NiagaraComponent->SetAutoActivate(true);
		}
	}
}