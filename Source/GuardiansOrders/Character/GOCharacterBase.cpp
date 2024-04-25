// Fill out your copyright notice in the Description page of Project Settings.


#include "GOCharacterBase.h"
#include "UI/GOWidgetComponent.h" 
#include "UI/GOStatsBarWidget.h"
#include "UI/GOHpBarWidget.h"
#include "UI/GOManaBarWidget.h"
#include "Skill/GOSkillCastComponent.h"
#include "CharacterStat/GOCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

AGOCharacterBase::AGOCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 95.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_GOCAPSULE);
	GetCapsuleComponent()->bReceivesDecals = false;

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->bEnablePhysicsInteraction = false;
	// GetCharacterMovement()->SlideAlongSurface(false);

	// Mesh
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->bReceivesDecals = false;

	// Stat Component
	Stat = CreateDefaultSubobject<UGOCharacterStatComponent>(TEXT("Stat"));

	//// Widget Component : HP
	//HpBar = CreateDefaultSubobject<UGOWidgetComponent>(TEXT("HpBarWidget"));
	//HpBar->SetupAttachment(GetMesh());
	//HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 230.0f));
	//static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/ProgressBar/WBP_HpBar.WBP_HpBar_C"));
	//if (HpBarWidgetRef.Succeeded())
	//{
	//	HpBar->SetWidgetClass(HpBarWidgetRef.Class);
	//	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	//	HpBar->SetDrawSize(FVector2D(130.0f, 15.0f));
	//	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}

	//// Widget Component : Mana
	//ManaBar = CreateDefaultSubobject<UGOWidgetComponent>(TEXT("ManaBarWidget"));
	//ManaBar->SetupAttachment(GetMesh());
	//ManaBar->SetRelativeLocation(FVector(0.0f, 0.0f, 215.0f));
	//static ConstructorHelpers::FClassFinder<UUserWidget> ManaBarWidgetRef(TEXT("/Game/UI/ProgressBar/WBP_ManaBar.WBP_ManaBar_C"));
	//if (ManaBarWidgetRef.Succeeded())
	//{
	//	ManaBar->SetWidgetClass(ManaBarWidgetRef.Class);
	//	ManaBar->SetWidgetSpace(EWidgetSpace::Screen);
	//	ManaBar->SetDrawSize(FVector2D(130.0f, 15.0f));
	//	ManaBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}

	StatsBar = CreateDefaultSubobject<UGOWidgetComponent>(TEXT("StatsBarWidget"));
	StatsBar->SetupAttachment(GetMesh());
	StatsBar->SetRelativeLocation(FVector(0.0f, 0.0f, 215.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> StatsBarWidgetRef(TEXT("/Game/UI/ProgressBar/WBP_StatsBar.WBP_StatsBar_C"));
	if (StatsBarWidgetRef.Succeeded())
	{
		StatsBar->SetWidgetClass(StatsBarWidgetRef.Class);
		StatsBar->SetWidgetSpace(EWidgetSpace::Screen);
		StatsBar->SetDrawSize(FVector2D(130.0f, 30.0f));
		StatsBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	 }

	ConstructorHelpers::FObjectFinder<UDataTable> CharacterDataObj(TEXT("DataTable'/Game/GameData/CharacterDataTable/GOCharacterDataTable.GOCharacterDataTable'"));
	if (CharacterDataObj.Succeeded())
	{
		CharacterDataTable = CharacterDataObj.Object;
	}	

	SkillCastComponent = CreateDefaultSubobject<UGOSkillCastComponent>(TEXT("SkillCastComponent"));
}

void AGOCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// GetCharacterMovement()->MaxWalkSpeed = Stat->GetTotalStat().MovementSpeed;
	Stat->OnHpZero.AddUObject(this, &AGOCharacterBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AGOCharacterBase::ApplyStat);
	Stat->OnManaZero.AddUObject(this, &AGOCharacterBase::NoMana);
}

void AGOCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGOCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGOCharacterBase::SetData(FName InCharacterName)
{
	// Character Data Lookup
	static const FString ContextString(TEXT("Character Data Lookup"));

	FGOCharacterData* CharacterDataRow = CharacterDataTable->FindRow<FGOCharacterData>(InCharacterName, ContextString, true);
	if (CharacterDataRow)
	{
		CharacterData = *CharacterDataRow;

		// SetCharacterStatData(InCharacterName);
		if (Stat)
		{
			Stat->SetCharacterStat(InCharacterName);
		}

		BaseSkillClass = CharacterData.BaseSkillClass;
		SkillQClass = CharacterData.SkillQClass;
		SkillWClass = CharacterData.SkillWClass;
		SkillEClass = CharacterData.SkillEClass;
		SkillRClass = CharacterData.SkillRClass;

		// 스킬 데이터 테이블의 RowName
		SetBaseSkillData(CharacterData.DefaultBaseSkillName);
		SetSkillDataQ(CharacterData.DefaultSkillNameQ);
		SetSkillDataW(CharacterData.DefaultSkillNameW);
		SetSkillDataE(CharacterData.DefaultSkillNameE);
		SetSkillDataR(CharacterData.DefaultSkillNameR);

		GetMesh()->SetSkeletalMesh(CharacterData.SkeletalMesh);
		GetMesh()->SetAnimInstanceClass(CharacterData.AnimBlueprint);

		GetCharacterMovement()->MaxWalkSpeed = Stat->GetTotalStat().MovementSpeed;
	}
}

void AGOCharacterBase::SetCharacterStatData(FName InCharacterName)
{
	// Character Stat Data Lookup
	static const FString ContextString(TEXT("Character Stat Data Lookup"));

	if (!CharacterStatDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterStatDataTable is not initialized."));
		return;
	}

	FGOCharacterStat* CharacterStatDataRow = CharacterStatDataTable->FindRow<FGOCharacterStat>(InCharacterName, ContextString, true);
	if (CharacterStatDataRow)
	{
		CharacterStat = *CharacterStatDataRow;

		// StatComponent 처리
		Stat->SetBaseStat(CharacterStat);
	}
}

void AGOCharacterBase::SetBaseSkillData(FName InSkillName)
{
	if (BaseSkillClass != nullptr)
	{
		BaseSkillInstance = NewObject<UGOSkillBase>(this, BaseSkillClass);
		if (BaseSkillInstance)
		{
			BaseSkillInstance->InitializeSkill(InSkillName);
			BaseSkillInstance->SetSkillOwner(this);

			SkillSlot.Add(BaseSkillInstance);
		}
	}
}

void AGOCharacterBase::SetSkillDataQ(FName InSkillName)
{
	if (SkillQClass != nullptr)
	{
		SkillQInstance = NewObject<UGOSkillBase>(this, SkillQClass);
		if (SkillQInstance)
		{
			SkillQInstance->InitializeSkill(InSkillName);
			SkillQInstance->SetSkillOwner(this);

			SkillSlot.Add(SkillQInstance);
		}
	}
}

void AGOCharacterBase::SetSkillDataW(FName InSkillName)
{
	if (SkillWClass != nullptr)
	{
		SkillWInstance = NewObject<UGOSkillBase>(this, SkillWClass);
		if (SkillWInstance)
		{
			SkillWInstance->InitializeSkill(InSkillName);
			SkillWInstance->SetSkillOwner(this);

			SkillSlot.Add(SkillWInstance);
		}
	}
}

void AGOCharacterBase::SetSkillDataE(FName InSkillName)
{
	if (SkillEClass != nullptr)
	{
		SkillEInstance = NewObject<UGOSkillBase>(this, SkillEClass);
		if (SkillEInstance)
		{
			SkillEInstance->InitializeSkill(InSkillName);
			SkillEInstance->SetSkillOwner(this);
		
			SkillSlot.Add(SkillEInstance);
		}
	}
}

void AGOCharacterBase::SetSkillDataR(FName InSkillName)
{
	if (SkillRClass != nullptr)
	{
		SkillRInstance = NewObject<UGOSkillBase>(this, SkillRClass);
		if (SkillRInstance)
		{
			SkillRInstance->InitializeSkill(InSkillName);
			SkillRInstance->SetSkillOwner(this);

			SkillSlot.Add(SkillRInstance);
		}
	}
}

void AGOCharacterBase::ApplyStat(const FGOCharacterStat& BaseStat, const FGOCharacterStat& ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AGOCharacterBase::SetupCharacterWidget(UGOUserWidget* InUserWidget)
{
	//UGOHpBarWidget* HpBarWidget = Cast<UGOHpBarWidget>(InUserWidget);
	//if (HpBarWidget)
	//{
	//	// HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
	//	HpBarWidget->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());
	//	Stat->OnHpChanged.AddUObject(HpBarWidget, &UGOHpBarWidget::UpdateHpBar);
	//	// Stat->OnStatChanged.AddUObject(HpBarWidget, &UGOHpBarWidget::UpdateStat);
	//}

	//UGOManaBarWidget* ManaBarWidget = Cast<UGOManaBarWidget>(InUserWidget);
	//if (ManaBarWidget)
	//{
	//	// ManaBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
	//	ManaBarWidget->UpdateManaBar(Stat->GetCurrentMana(), Stat->GetMaxMana());
	//	Stat->OnManaChanged.AddUObject(ManaBarWidget, &UGOManaBarWidget::UpdateManaBar);
	//	// Stat->OnStatChanged.AddUObject(ManaBarWidget, &UGOManaBarWidget::UpdateStat);
	//}

	UGOStatsBarWidget* StatsBarWidget = Cast<UGOStatsBarWidget>(InUserWidget);
	if (StatsBarWidget)
	{
		// StatsBar 위젯 안의 HP바와 마나바에 대한 참조를 가져옵니다.
		//UGOHpBarWidget* HpBarWidget = Cast<UGOHpBarWidget>(StatsBarWidget->HpBar);
		//UGOManaBarWidget* ManaBarWidget = Cast<UGOManaBarWidget>(StatsBarWidget->ManaBar);
		UGOHpBarWidget* HpBarWidget = Cast<UGOHpBarWidget>(StatsBarWidget->GetWidgetFromName(TEXT("PbHpBar")));
		UGOManaBarWidget* ManaBarWidget = Cast<UGOManaBarWidget>(StatsBarWidget->GetWidgetFromName(TEXT("PbManaBar")));
		
		if (HpBarWidget && ManaBarWidget)
		{
			// HP와 마나 업데이트 함수를 상태 변경 이벤트에 바인딩합니다.
			HpBarWidget->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());
			ManaBarWidget->UpdateManaBar(Stat->GetCurrentMana(), Stat->GetMaxMana());
			Stat->OnHpChanged.AddUObject(HpBarWidget, &UGOHpBarWidget::UpdateHpBar);
			Stat->OnManaChanged.AddUObject(ManaBarWidget, &UGOManaBarWidget::UpdateManaBar);
		}
	}
}

void AGOCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float DamageRange = Stat->GetTotalStat().DamageRange;
	const float DamageRadius = Stat->GetTotalStat().DamageRadius;
	const float BaseDamage = Stat->GetTotalStat().BaseDamage;
	const float DamageSpeed = Stat->GetTotalStat().DamageSpeed;

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * DamageRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_GOACTION, FCollisionShape::MakeSphere(DamageRadius), Params);

	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(BaseDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = DamageRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, DamageRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

float AGOCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);
	// SetDead();

	return DamageAmount;
}

void AGOCharacterBase::ProcessComboCommand()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage);
}

void AGOCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
    if (HpBar) // Ensure HpBar is not null
        HpBar->SetHiddenInGame(true);
    
    if (ManaBar) // Ensure ManaBar is not null
        ManaBar->SetHiddenInGame(true);
	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//if (PlayerController)
	//{
	//	DisableInput(PlayerController);
	//}
}

void AGOCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AGOCharacterBase::GetMana()
{
}

void AGOCharacterBase::NoMana()
{
	// TODO: No Mana
}

