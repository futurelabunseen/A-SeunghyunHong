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

AGOPlayerCharacter::AGOPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGOCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)),
	bIsDecalVisible(false)
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

	// Skill Cast Component
	// SkillCastComponent = CreateDefaultSubobject<UGOSkillCastComponent>(TEXT("SkillCastComponent"));

	// Character State Init
	ActionStateBitMask = EGOPlayerActionState::None;
}

void AGOPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
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

		EnhancedInputComponent->BindAction(ActionBaseSkill, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnBaseSkill);
		EnhancedInputComponent->BindAction(ActionSkillQ, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::OnSkillQ);
		//EnhancedInputComponent->BindAction(ActionSkillQ, ETriggerEvent::Triggered, this, &AGOPlayerCharacter::Attack);
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

void AGOPlayerCharacter::OnBaseSkill()
{
	//SkillCastComponent->OnStartCast(
	//	CharacterSkillSet->GetSkill(ECharacterSkills::BaseSkill)
	//);	

	SkillCastComponent->OnStartCast(
		FHeroSkillKey(CharacterData.HeroType, ECharacterSkills::BaseSkill));
}

void AGOPlayerCharacter::OnSkillQ()
{
	//SkillCastComponent->OnStartCast(
	// CharacterSkillSet->GetSkill(ECharacterSkills::Skill01)
	// );

	SkillCastComponent->OnStartCast(
		FHeroSkillKey(CharacterData.HeroType, ECharacterSkills::Skill01));
}

void AGOPlayerCharacter::OnSkillW()
{
	//SkillCastComponent->OnStartCast(
	// CharacterSkillSet->GetSkill(ECharacterSkills::Skill02)
	// );

	SkillCastComponent->OnStartCast(
		FHeroSkillKey(CharacterData.HeroType, ECharacterSkills::Skill02));

}

void AGOPlayerCharacter::OnSkillE()
{
	//SkillCastComponent->OnStartCast(
	// CharacterSkillSet->GetSkill(ECharacterSkills::Skill03)
	// );

	SkillCastComponent->OnStartCast(
		FHeroSkillKey(CharacterData.HeroType, ECharacterSkills::Skill03));
}

void AGOPlayerCharacter::OnSkillR()
{
	//SkillCastComponent->OnStartCast(
	// CharacterSkillSet->GetSkill(ECharacterSkills::UltimateSkill)
	// );

	SkillCastComponent->OnStartCast(
		FHeroSkillKey(CharacterData.HeroType, ECharacterSkills::UltimateSkill));
}

// Flash Spell : 마우스 커서가 있는 쪽으로 짧은 순간이동이 가능합니다.
void AGOPlayerCharacter::OnSpellD()
{
	//SpellCastComponent->OnStartCast(
	//	FHeroSpellKey(CharacterData.HeroType, ECharacterSpells::Spell01));

	UGOCharacterMovementComponent* GOMovement = Cast<UGOCharacterMovementComponent>(GetCharacterMovement());
	if (GOMovement)
	{
		GOMovement->SetFlashSpellCommand();
	}

	UE_LOG(LogTemp, Log, TEXT("Common Spell D is triggered. FLASH "));
}

// Heal Spell
void AGOPlayerCharacter::OnSpellF()
{
	if (HasAuthority())
	{
		Stat->HealHp();
	}
	else
	{
		Stat->ServerHealHp();
	}

	SpellCastComponent->OnStartCast(
		FHeroSpellKey(CharacterData.HeroType, ECharacterSpells::Spell02));

	UE_LOG(LogTemp, Log, TEXT("Common Spell F is triggered. HEAL : Up to 20 percent of HP recovers. "));
}

// Ghost Spell
void AGOPlayerCharacter::OnSpellG()
{
	/*SpellCastComponent->OnStartCast(
		FHeroSpellKey(CharacterData.HeroType, ECharacterSpells::Spell03));*/

	UGOCharacterMovementComponent* GOMovement = Cast<UGOCharacterMovementComponent>(GetCharacterMovement());
	if (GOMovement)
	{
		GOMovement->SetGhostSpellCommand();
	}

	UE_LOG(LogTemp, Log, TEXT("Common Spell G is triggered. GHOST :  "));
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

void AGOPlayerCharacter::UpdateSkillBar()
{
	//auto Skills = CharacterSkillSet->GetSkills();
	//UGOSkillSetBarWidget* SkillSetBar = Cast<UGOSkillSetBarWidget>(GetSkillBarWidget());
	//if (!SkillSetBar || !CharacterSkillSet || Skills.Num() == 0)
	//{
	//	return;
	//}

	//// SkillSlots 배열의 크기를 스킬 개수에 맞게 조정
	//for (int32 i = 0; i < SkillSetBar->SkillSlots.Num() && i < Skills.Num(); ++i)
	//{
	//	if (Skills[i].GOSkillData && SkillSetBar->SkillSlots[i])
	//	{
	//		UTexture2D* SkillIcon = CharacterSkillSet->GetSkills()[i].GOSkillData->SkillIcon;
	//		if (SkillIcon)
	//		{
	//			SkillSetBar->SkillSlots[i]->SetBrushFromTexture(SkillIcon, true);
	//		}
	//	}
	//}
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

	DOREPLIFETIME(AGOPlayerCharacter, ActionStateBitMask);

	DOREPLIFETIME(AGOPlayerCharacter, BlownRecoveryTimer);
	DOREPLIFETIME(AGOPlayerCharacter, InvincibleTime);
	DOREPLIFETIME(AGOPlayerCharacter, InvincibleTimer);
	DOREPLIFETIME(AGOPlayerCharacter, ImpactTimer);
}

void AGOPlayerCharacter::Attack()
{
	// ProcessComboCommand();

	if (bCanAttack)// 상태 체크
	{
		// 클라이언트에게 애니메이션 재생, 타이머 재생을 맡깁니다.
		if (!HasAuthority())
		{
			bCanAttack = false;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

			GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AGOPlayerCharacter::ResetAttack, AttackTime, false);
			//FTimerHandle Handle;
			//GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			//	{
			//		bCanAttack = true;
			//		// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			//	}
			//), AttackTime, false, -1.0f);
			UE_LOG(LogTemp, Log, TEXT("요기1: Attack() ")); // 클라는 요기1, 요기2 // 서버는 요기2, 요기3

			PlayAttackAnimation(); //요기

		}

		// 서버에게 서버시간과 함께 명령을 보냅니다.
		ServerRPCAttack(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}

// 원래 강의에서 몽타주 실행하는 코드
void AGOPlayerCharacter::PlayAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(ComboActionMontage);
}

// 애니메이션이 재생되면 애니메이션의 노티파이가 !
void AGOPlayerCharacter::AttackHitCheck()
{
	// 소유권을 가진 클라이언트가 공격 판정을 진행합니다.  
	if (IsLocallyControlled())
	{
		GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
		FHitResult OutHitResult;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

		// TODO 스킬 스탯을 가져와야 하는데? 어떻게? 가져오지?

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

				UE_LOG(LogTemp, Warning, TEXT("before HitCameraShakeClass"));

				if (HitCameraShakeClass)
				{
					GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
					UE_LOG(LogTemp, Warning, TEXT("HitCameraShakeClass"));
				}
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
				// AttackHitConfirm(OutHitResult.GetActor());

				if (HitCameraShakeClass)
				{
					GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
					UE_LOG(LogTemp, Warning, TEXT("HitCameraShakeClass"));
				}
			}

		}
	}
}

// 새로 만든: 스킬시스템용 
void AGOPlayerCharacter::SkillAttackHitCheck()
{
	// 소유권을 가진 클라이언트가 공격 판정을 진행합니다.  
	if (IsLocallyControlled())
	{
		GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
		
		// TODO : nullptr 오류 체크: casting time 과 관련 있음

		TObjectPtr<UGOSkillBase> CurrentSkill = GetSkillCastComponent()->GetCurrentSkill();
		bool HitDetected = CurrentSkill->GetHitDetected();
		UE_LOG(LogTemp, Warning, TEXT("CurrentSkill Hit: %s"), *CurrentSkill->GetName());

		FGOOutHitCollisionStructure& OutHitCollisionStructure = CurrentSkill->GetOutHitCollisionStructure();
		FGOOutHitCollisionStructure SkillHitCollisionStructure = OutHitCollisionStructure; // 추가

		FHitResult OutHitResult = OutHitCollisionStructure.OutHitResult;
		TArray<FHitResult> OutHitResults = OutHitCollisionStructure.OutHitResults;
		TArray<FOverlapResult> OutOverlaps = OutHitCollisionStructure.OutOverlaps;

		ESkillCollisionType CurrentSkillCollisionType = CurrentSkill->GetSkillCollisionType();
		float SkillDamage = CurrentSkill->GetTotalSkillStat().DamageMultiplier * Stat->GetTotalStat().BaseDamage;

		// 공격한 시간
		float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("[HIT!!] BEFORE case ESkillCollisionType::OverlapMulti:"));

		// 클라이언트에서 진행 (판정에 대한 검증을 받아야 하므로 패킷 전송 필요)
		if (!HasAuthority())
		{
			if (HitDetected)
			{
				UE_LOG(LogTemp, Warning, TEXT("[HIT!!] BEFORE 222 case ESkillCollisionType::OverlapMulti:"));

				AActor* HitActor = nullptr;
				switch (CurrentSkillCollisionType)
				{
				case ESkillCollisionType::LineTraceSingle:
					HitActor = OutHitCollisionStructure.OutHitResult.GetActor();
					if (IsValid(HitActor)) {
						UE_LOG(LogTemp, Warning, TEXT("[HIT!!] LineTraceSingle AttackSkillHitConfirm Hit: %s, %f"), *HitActor->GetName(), SkillDamage);
						ServerRPCNotifySkillHitTest(OutHitResult, SkillDamage);
					}
					break;

				case ESkillCollisionType::LineTraceMulti:

					ServerRPCNotifySkillHitResults(OutHitResults, SkillDamage);

					for (const FHitResult& HitResult : OutHitCollisionStructure.OutHitResults) {
						HitActor = HitResult.GetActor();
						if (IsValid(HitActor)) {
							UE_LOG(LogTemp, Warning, TEXT("[HIT!!] LineTraceMulti AttackSkillHitConfirm Hit: %s, %f"), *HitActor->GetName(), SkillDamage);
						}
					}
					break;

				case ESkillCollisionType::SweepSingle:
					UE_LOG(LogTemp, Warning, TEXT("Hit: 3333 sweep single"));

					HitActor = OutHitCollisionStructure.OutHitResult.GetActor();

					UE_LOG(LogTemp, Warning, TEXT("Hit: 4444 sweep single"));

					if (HitActor != nullptr) {
						UE_LOG(LogTemp, Warning, TEXT("Hit: 5555 sweep single"));
						 ServerRPCNotifySkillHitTest(OutHitResult, SkillDamage);
						UE_LOG(LogTemp, Warning, TEXT("[HIT!!] SweepSingle AttackSkillHitConfirm Hit: %s, %f"), *HitActor->GetName(), SkillDamage);
					}
					break;

				case ESkillCollisionType::SweepMulti:

					ServerRPCNotifySkillHitResults(OutHitResults, SkillDamage);

					for (const FHitResult& HitResult : OutHitCollisionStructure.OutHitResults) {
						HitActor = HitResult.GetActor();
						if (IsValid(HitActor)) {
							UE_LOG(LogTemp, Warning, TEXT("[HIT!!] SweepMulti AttackSkillHitConfirm Hit: %s, %f"), *HitActor->GetName(), SkillDamage);
						}
					}
					break;

				case ESkillCollisionType::OverlapMulti:
					UE_LOG(LogTemp, Warning, TEXT("[HIT!!] case ESkillCollisionType::OverlapMulti:"));

					ServerRPCNotifySkillHitOverlapResult(OutOverlaps, SkillDamage);

					for (const FOverlapResult& OverlapResult : OutHitCollisionStructure.OutOverlaps) {
						HitActor = OverlapResult.GetActor();
						if (IsValid(HitActor)) {
							UE_LOG(LogTemp, Warning, TEXT("[HIT!!] OverlapMulti AttackSkillHitConfirm Hit: %s, %f"), *HitActor->GetName(), SkillDamage);
						}
					}
					break;

				default:
					UE_LOG(LogTemp, Warning, TEXT("[HIT!!] Unknown collision type!!!"));
					break;
				}
				if (HitCameraShakeClass)
				{
					GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
				}
			}
			else
			{
				ServerRPCNotifySkillMiss(HitCheckTime);
			}
		}
		// 서버에서 진행 (패킷 전송 필요없이 바로 처리)
		else
		{
			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
			/// DrawDebugAttackRange(DebugColor, Start, End, Forward);

			if (HitDetected)
			{
				AActor* HitActor = nullptr;
				switch (CurrentSkillCollisionType)
				{
				case ESkillCollisionType::LineTraceSingle:
					HitActor = SkillHitCollisionStructure.OutHitResult.GetActor();
					if (IsValid(HitActor)) {
						AttackSkillHitConfirm(HitActor, SkillDamage);
						UE_LOG(LogTemp, Warning, TEXT("[HIT!!] LineTraceSingle AttackSkillHitConfirm Hit: %s, %d"), *HitActor->GetName(), SkillDamage);
					}
					break;

				case ESkillCollisionType::LineTraceMulti:
					for (const FHitResult& HitResult : SkillHitCollisionStructure.OutHitResults) {
						HitActor = HitResult.GetActor();
						if (IsValid(HitActor)) {
							AttackSkillHitConfirm(HitActor, SkillDamage);
							UE_LOG(LogTemp, Warning, TEXT("[HIT!!] LineTraceMulti AttackSkillHitConfirm Hit: %s, %d"), *HitActor->GetName(), SkillDamage);
						}
					}
					break;

				case ESkillCollisionType::SweepSingle:
					HitActor = SkillHitCollisionStructure.OutHitResult.GetActor();
					if (HitActor != nullptr) {
						AttackSkillHitConfirm(OutHitResult.GetActor(), SkillDamage);
						UE_LOG(LogTemp, Warning, TEXT("[HIT!!] SweepSingle AttackSkillHitConfirm Hit: %s, %d"), *HitActor->GetName(), SkillDamage);

					}
					break;

				case ESkillCollisionType::SweepMulti:
					for (const FHitResult& HitResult : SkillHitCollisionStructure.OutHitResults) {
						HitActor = HitResult.GetActor();
						if (IsValid(HitActor)) {
							AttackSkillHitConfirm(HitActor, SkillDamage);
							UE_LOG(LogTemp, Warning, TEXT("[HIT!!] SweepMulti AttackSkillHitConfirm Hit: %s, %d"), *HitActor->GetName(), SkillDamage);
						}
					}
					break;

				case ESkillCollisionType::OverlapMulti:
					for (const FOverlapResult& OverlapResult : SkillHitCollisionStructure.OutOverlaps) {
						HitActor = OverlapResult.GetActor();
						if (IsValid(HitActor)) {
							AttackSkillHitConfirm(HitActor, SkillDamage);
							UE_LOG(LogTemp, Warning, TEXT("[HIT!!] OverlapMulti AttackSkillHitConfirm Hit: %s, %d"), *HitActor->GetName(), SkillDamage);
						}
					}
					break;

				default:
					UE_LOG(LogTemp, Warning, TEXT("Unknown collision type!!!"));
					break;
				}

				if (HitCameraShakeClass)
				{
					GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
					UE_LOG(LogTemp, Warning, TEXT("HitCameraShakeClass"));
				}
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
		HitActor->TakeDamage(100, DamageEvent, GetController(), this); //?
	}
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));

}

// 새로 만든: 스킬시스템용 
void AGOPlayerCharacter::AttackSkillHitConfirm(AActor* HitActor, float SkillDamage)
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	UE_LOG(LogTemp, Warning, TEXT("ServerRPCNotifySkillHit_Implementation !!! AttackSkillHitConfirm SkillDamage: %f"), SkillDamage);

	if (HasAuthority())
	{
		FDamageEvent DamageEvent;
		HitActor->TakeDamage(SkillDamage, DamageEvent, GetController(), this);
		UE_LOG(LogTemp, Warning, TEXT("ServerRPCNotifySkillHit_Implementation !!!  SkillDamage: %f"), SkillDamage);
	}

	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));
}

void AGOPlayerCharacter::DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
#if ENABLE_DRAW_DEBUG
	const float DamageRange = Stat->GetTotalStat().DamageRange;
	const float DamageRadius = Stat->GetTotalStat().DamageRadius;
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

// 새로 만든: 스킬시스템용 
void AGOPlayerCharacter::ClientRPCPlaySkillAnimation_Implementation(AGOPlayerCharacter* CharacterToPlay, UGOSkillBase* CurrentSkill)
{
	if (CharacterToPlay)
	{
		if (CurrentSkill == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ClientRPCPlaySkillAnimation_Implementation] CurrentSkill is null."));
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

bool AGOPlayerCharacter::ServerRPCNotifyHit_Validate(const FHitResult& HitResult, float HitChecktime)
{
	// return (HitChecktime - LastAttakStartTime) > AttackTime;

	// 원래 이거
	// return (HitChecktime - LastAttakStartTime) >= AcceptMinCheckTime;
	return true;
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
			UGOSkillBase* ActiveSkill = SkillCastComponent->GetCurrentSkill();
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

// 새로 만든: 스킬시스템용 
bool AGOPlayerCharacter::ServerRPCNotifySkillHit_Validate(const FGOOutHitCollisionStructure SkillHitCollisionStructure, float HitChecktime, ESkillCollisionType CurrentSkillCollisionType, float DamageAmount)
{
	// return (HitChecktime - LastAttakStartTime) > AttackTime;

	// 원래 이거
	// return (HitChecktime - LastAttakStartTime) >= AcceptMinCheckTime;
	return true;
}

// 새로 만든: 스킬시스템용 
void AGOPlayerCharacter::ServerRPCNotifySkillHit_Implementation(const FGOOutHitCollisionStructure SkillHitCollisionStructure, float HitChecktime, ESkillCollisionType CurrentSkillCollisionType, float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHit_Implementation !!! "));
	UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHit_Implementation !!!  SkillDamage: %f, CurrentSkillCollisionType: %d "), DamageAmount, CurrentSkillCollisionType);

	AActor* HitActor = nullptr; 
	switch (CurrentSkillCollisionType)
	{
	case ESkillCollisionType::LineTraceSingle:
		HitActor = SkillHitCollisionStructure.OutHitResult.GetActor();
		if (IsValid(HitActor)) {
			AttackSkillHitConfirm(HitActor, DamageAmount);
			UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHit_Implementation LineTraceSingle!!! Hit: %s, SkillDamage: %f"), *HitActor->GetName(), DamageAmount);
		}
		break;

	case ESkillCollisionType::LineTraceMulti:
		for (const FHitResult& HitResult : SkillHitCollisionStructure.OutHitResults) {
			HitActor = HitResult.GetActor();
			if (IsValid(HitActor)) {
				AttackSkillHitConfirm(HitActor, DamageAmount);
				UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHit_Implementation LineTraceMulti!!! Hit: %s, SkillDamage: %f"), *HitActor->GetName(), DamageAmount);
			}
		}
		break;

	case ESkillCollisionType::SweepSingle:
		UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHit_Implementation SweepSingle!!! 000"));

		HitActor = SkillHitCollisionStructure.OutHitResult.GetActor();
		if (HitActor != nullptr) {
			AttackSkillHitConfirm(HitActor, DamageAmount);
			UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHit_Implementation SweepSingle!!! Hit: %s, SkillDamage: %f"), *HitActor->GetName(), DamageAmount);
		}
		break;

	case ESkillCollisionType::SweepMulti:
		for (const FHitResult& HitResult : SkillHitCollisionStructure.OutHitResults) {
			HitActor = HitResult.GetActor();
			if (IsValid(HitActor)) {
				AttackSkillHitConfirm(HitActor, DamageAmount);
				UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHit_Implementation SweepMulti!!! Hit: %s, SkillDamage: %f"), *HitActor->GetName(), DamageAmount);
			}
		}
		break;

	case ESkillCollisionType::OverlapMulti:
		for (const FOverlapResult& OverlapResult : SkillHitCollisionStructure.OutOverlaps) {
			HitActor = OverlapResult.GetActor();
			if (IsValid(HitActor)) {
				AttackSkillHitConfirm(HitActor, DamageAmount);
				UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHit_Implementation OverlapMulti!!! Hit: %s, SkillDamage: %f"), *HitActor->GetName(), DamageAmount);
			}
		}
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown collision type!!!"));
		break;
	}
}

// 테스트용: 스킬시스템
bool AGOPlayerCharacter::ServerRPCNotifySkillHitTest_Validate(const FHitResult& HitResult, float DamageAmount)
{
	return true;
}

// 테스트용: 스킬시스템
void AGOPlayerCharacter::ServerRPCNotifySkillHitTest_Implementation(const FHitResult& HitResult, float DamageAmount)
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	AActor* HitActor = HitResult.GetActor();
	if (::IsValid(HitActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHitTest_Implementation !!! Hit: %s, SkillDamage: %f"), *HitActor->GetName(), DamageAmount);

		const FVector HitLocation = HitResult.Location;
		const FBox HitBox = HitActor->GetComponentsBoundingBox();
		const FVector ActorBoxCenter = (HitBox.Min + HitBox.Max) * 0.5f;
		UE_LOG(LogTemp, Warning, TEXT("[HIT!!] Unknown collision type!!!"));
		AttackSkillHitConfirm(HitActor, DamageAmount);
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

bool AGOPlayerCharacter::ServerRPCNotifyMiss_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	// Beast Skill E 오류가 나서 우선 true로 설정함
	// return (HitCheckTime - LastAttakStartTime) > AcceptMinCheckTime;
	return true;
}

void AGOPlayerCharacter::ServerRPCNotifyMiss_Implementation(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	DrawDebugAttackRange(FColor::Red, TraceStart, TraceEnd, TraceDir);
}

// 새로 만든: 스킬시스템용 
bool AGOPlayerCharacter::ServerRPCNotifySkillMiss_Validate(float HitCheckTime)
{
	// Beast Skill E 오류가 나서 우선 true로 설정함
	// return (HitCheckTime - LastAttakStartTime) > AcceptMinCheckTime;
	return true;
}

// 새로 만든: 스킬시스템용 
void AGOPlayerCharacter::ServerRPCNotifySkillMiss_Implementation(float HitCheckTime)
{
	// DrawDebugAttackRange(FColor::Red, TraceStart, TraceEnd, TraceDir);
}

void AGOPlayerCharacter::ServerRPCNotifySkillHitResults_Implementation(const TArray<FHitResult>& HitResult, float DamageAmount)
{
	AActor* HitActor = nullptr;

	for (const FHitResult& HitResult : HitResult) {
		HitActor = HitResult.GetActor();
		if (IsValid(HitActor)) {
			AttackSkillHitConfirm(HitActor, DamageAmount);
			UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHitResults_Implementation Hit: %s, %f"), *HitActor->GetName(), DamageAmount);
		}
	}
}

void AGOPlayerCharacter::ServerRPCNotifySkillHitOverlapResult_Implementation(const TArray<FOverlapResult>& FOverlapResults, float DamageAmount)
{
	AActor* HitActor = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHitOverlapResult_Implementation 000 Hit: %f"), DamageAmount);

	for (const FOverlapResult& OverlapResult : FOverlapResults) {
		HitActor = OverlapResult.GetActor();
		if (IsValid(HitActor)) {
			UE_LOG(LogTemp, Warning, TEXT("[HIT!!] ServerRPCNotifySkillHitOverlapResult_Implementation 111 Hit: %s"), *HitActor->GetName());

			AttackSkillHitConfirm(HitActor, DamageAmount);
			UE_LOG(LogTemp, Warning, TEXT("[HIT!!] OverlapMulti AttackSkillHitConfirm Hit: %s, %f"), *HitActor->GetName(), DamageAmount);
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
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AGOPlayerCharacter::SetDead()
{
	Super::SetDead();
	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &AGOPlayerCharacter::ResetPlayer, 5.0f, false);
}

float AGOPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Stat->GetCurrentHp() <= 0.0f)
	{
		IGOBattleInterface* GOBattleMode = GetWorld()->GetAuthGameMode<IGOBattleInterface>();
		if (GOBattleMode)
		{
			GOBattleMode->OnPlayerKilled(EventInstigator, GetController(), this);
		}
	}
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("End"));

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

	// return (AttackStartTime - LastAttakStartTime) > (AttackTime - 0.4f);
	return true;
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
	PlayAttackAnimation(); //요기

	UE_LOG(LogTemp, Log, TEXT("요기2: ServerRPCAttack_Implementation "));

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
					OtherPlayer->ClientRPCPlayAnimation(this); //요기
					UE_LOG(LogTemp, Log, TEXT("요기3: Attack() "));

				}
			}
		}
	}
}

// 새로 만든: 스킬시스템용 
bool AGOPlayerCharacter::ServerRPCAttackNew_Validate(float AttackStartTime, UGOSkillBase* CurrentSkill)
{
	//if (LastAttakStartTime == 0.0f)
	//{
	//	return true;
	//}

	//// return (AttackStartTime - LastAttakStartTime) > (AttackTime - 0.4f);
	return true;
}

// 새로 만든: 스킬시스템용 
void AGOPlayerCharacter::ServerRPCAttackNew_Implementation(float AttackStartTime, UGOSkillBase* CurrentSkill)
{
	GO_LOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	Stat->UseSkill(Stat->GetTotalStat().BaseDamage); // TODO: ManaCost

	// 프로퍼티 OnRep 함수를 명시적 호출합니다.
	////OnRep_CanAttack();

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

	// PlayAttackAnimation(); //요기

	UE_LOG(LogTemp, Log, TEXT("[CheckActorNetworkStatus ServerRPCAttackNew] Actor %s is "), *CurrentSkill->GetName());

	PlaySkillAnim(CurrentSkill); // 여기서도 null...

	UE_LOG(LogTemp, Log, TEXT("요기2: ServerRPCAttack_Implementation "));

	// MulticastRPCAttackNew(InSkillSlot);
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
					OtherPlayer->ClientRPCPlaySkillAnimation(this, CurrentSkill); // 여기가 안됨
				}
			}
		}
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
		Stat->UseSkill(GOGameInstance->GetSkillByHeroSkillKey(Key)->GetTotalSkillStat().ManaCost);

	}

	// 프로퍼티 OnRep 함수를 명시적 호출합니다.
	////OnRep_CanAttack();

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

	// PlayAttackAnimation(); //요기

	//UE_LOG(LogTemp, Log, TEXT("[CheckActorNetworkStatus ServerRPCAttackNew] Actor %s is "), *CurrentSkill->GetName());

	//PlaySkillAnim(CurrentSkill); // 여기서도 null...
	PlaySkillAnimByKey(Key);

	UE_LOG(LogTemp, Log, TEXT("요기2: ServerRPCAttack_Implementation "));

	// MulticastRPCAttackNew(InSkillSlot);
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


//// (3) Multicast 는 게임과 무관한 효과를 재생하는 것이 좋습니다. 지금 코드에서는 사용하고 있지 않습니다.
void AGOPlayerCharacter::MulticastRPCAttack_Implementation()
{
	if (!IsLocallyControlled())
	{
		// 현재 클라이언트는 이미 모션을 재생했으므로
		// 다른 클라이언트의 프록시로써 동작하는 캐릭터에 대해서만 모션을 재생시킵니다.
		PlayAttackAnimation(); //요기
	}
}

// 지금 코드에서는 사용하고 있지 않습니다.
void AGOPlayerCharacter::MulticastRPCAttackNew_Implementation(UGOSkillBase* CurrentSkill)
{
	if (!IsLocallyControlled())
	{
		if (CurrentSkill == nullptr)
		{
			//  왜 NULL 이지?
			UE_LOG(LogTemp, Log, TEXT("CurrentSkill 0 null"));
		}

		//UE_LOG(LogTemp, Log, TEXT("InSkillSlot 0: %s "), *InSkillSlot->GetSkillInstance()->GetName());

		// 현재 클라이언트는 이미 모션을 재생했으므로
		// 다른 클라이언트의 프록시로써 동작하는 캐릭터에 대해서만 모션을 재생시킵니다.
		PlaySkillAnim(CurrentSkill); //요기
	}
}



void AGOPlayerCharacter::CheckActorNetworkStatus(AActor* ActorToCheck)
{
	if (!ActorToCheck)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckActorNetworkStatus: The actor to check is null."));
		return;
	}

	// Checking if the actor has a network owner
	bool bHasOwner = ActorToCheck->HasNetOwner();
	UE_LOG(LogTemp, Log, TEXT("[CheckActorNetworkStatus] Actor %s has a net owner: %s"), *ActorToCheck->GetName(), bHasOwner ? TEXT("Yes") : TEXT("No"));

	// Checking if the actor is a network startup actor
	bool bIsStartupActor = ActorToCheck->IsNetStartupActor();
	UE_LOG(LogTemp, Log, TEXT("[CheckActorNetworkStatus] Actor %s is a startup actor: %s"), *ActorToCheck->GetName(), bIsStartupActor ? TEXT("Yes") : TEXT("No"));
}



// ======== IGOPlaySkillAnimInterface ========
void AGOPlayerCharacter::PlaySkillAnim(UGOSkillBase* CurrentSkill)
{
	if (!CurrentSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PlaySkillAnim] CurrentSkill is null."));
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PlaySkillAnim] AnimInstance is null."));
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
		UE_LOG(LogTemp, Warning, TEXT("[PlaySkillAnim] AnimInstance is null."));
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

// no use
void AGOPlayerCharacter::ActivateSkill(UGOSkillBase* CurrentSkill)
{
	if (!HasAuthority())
	{
		PlaySkillAnim(CurrentSkill);
	}

	// 서버에게 명령을 보냅니다.
	ServerRPCAttackNew(GetWorld()->GetGameState()->GetServerWorldTimeSeconds(), CurrentSkill);
}

// ======== IGOSpellFlashInterface ========
// no use
void AGOPlayerCharacter::ActivateSpellFlash()
{
	//FVector TargetLocation =
	//	GetActorLocation()
	//	+ GetActorForwardVector() * FlashMovementOffset;

	//FVector TargetLocation =
	//	GetActorLocation()
	//	+ GetActorForwardVector() * CharacterSpellSet->GetSpell(ECharacterSpells::Spell01)->GetTotalSpellStat().MoveSpeedMultiplier;

	//TeleportTo(TargetLocation,
	//	GetActorRotation(),
	//	false, // 테스트?
	//	true); // 장애물 체크?

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
