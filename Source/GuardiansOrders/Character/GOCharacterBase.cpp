// Fill out your copyright notice in the Description page of Project Settings.


#include "GOCharacterBase.h"
#include "UI/GOWidgetComponent.h" 
#include "UI/GOStatsBarWidget.h"
#include "UI/GOHpBarWidget.h"
#include "UI/GOManaBarWidget.h"
#include "Skill/GOSkillCastComponent.h"
#include "Skill/GOSpellCastComponent.h"
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
#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include <Game/GOPlayerState.h>
#include "Interface/GOHighlightInterface.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include "CommonTextBlock.h"

AGOCharacterBase::AGOCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	bIsDead(false)
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
	//GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetMesh()->SetCollisionResponseToChannel(CCHANNEL_GOProjectile, ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->bReceivesDecals = false;
	GetMesh()->bRenderCustomDepth = false;
	GetMesh()->CustomDepthStencilValue = 250.f;

	// Stat Component
	Stat = CreateDefaultSubobject<UGOCharacterStatComponent>(TEXT("Stat"));
	SkillCastComponent = CreateDefaultSubobject<UGOSkillCastComponent>(TEXT("SkillCastComponent"));
	SpellCastComponent = CreateDefaultSubobject<UGOSpellCastComponent>(TEXT("SpellCastComponent"));
	CharacterSkillSet = CreateDefaultSubobject<UGOSkills>(TEXT("Skills"));
	CharacterSpellSet = CreateDefaultSubobject<UGOSpells>(TEXT("Spells"));

	{
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
	}

	StatsBar = CreateDefaultSubobject<UGOWidgetComponent>(TEXT("StatsBarWidget"));
	StatsBar->SetupAttachment(GetMesh());
	StatsBar->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	static ConstructorHelpers::FClassFinder<UGOStatsBarWidget> StatsBarWidgetRef(TEXT("/Game/UI/ProgressBar/WBP_HeadUpStatsBar.WBP_HeadUpStatsBar_C")); //UUserWidget
	
	//// hp bar images (Red, Green, Blue)
	//static ConstructorHelpers::FObjectFinder<UTexture2D> BlueTextureObj(TEXT("Engine.Texture2D'/Game/AssetResource/UI/LOL-StatsBar-Short-HP-Blue.LOL-StatsBar-Short-HP-Blue'"));
	////static ConstructorHelpers::FObjectFinder<UTexture2D> GreenTextureObj(TEXT("/Game/AssetResource/UI/LOL-StatsBar-Short-HP-Green.LOL-StatsBar-Short-HP-Green"));
	//static ConstructorHelpers::FObjectFinder<UTexture2D> RedTextureObj(TEXT("/Game/AssetResource/UI/LOL-StatsBar-Short-HP-Red.LOL-StatsBar-Short-HP-Red"));

	////// Assign textures to class members
	//BlueTexture = BlueTextureObj.Object;
	////GreenTexture = GreenTextureObj.Object;
	//RedTexture = RedTextureObj.Object;

	if (StatsBarWidgetRef.Succeeded())
	{
		StatsBar->SetWidgetClass(StatsBarWidgetRef.Class);
		StatsBar->SetWidgetSpace(EWidgetSpace::Screen);
		StatsBar->SetDrawSize(FVector2D(150.0f, 30.0f));
		StatsBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//UE_LOG(LogTemp, Warning, TEXT("StatsBarWidgetClass 0 : %s"));
	UE_LOG(LogTemp, Warning, TEXT("StatsBarWidgetClass 0 "));

	if (IsValid(StatsBarWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("StatsBarWidgetClass 1 : %s"), *StatsBarWidgetClass->GetName());

		StatsBar->SetWidgetClass(StatsBarWidgetClass);
		StatsBar->SetWidgetSpace(EWidgetSpace::Screen);
		StatsBar->SetDrawSize(FVector2D(200.0f, 50.0f));
		StatsBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("StatsBarWidgetClass 2 : %s"), *StatsBarWidgetClass->GetName());
	}
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
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();
	if (GOGameInstance)
	{
		static const FString ContextString(TEXT("Character Data Lookup"));
		FGOCharacterData* CharacterDataRow = GOGameInstance->GetCharacterData(InCharacterName);
		if (CharacterDataRow)
		{
			CharacterData = *CharacterDataRow;

			if (Stat)
			{
				Stat->SetCharacterStat(InCharacterName);
			}

			GetMesh()->SetSkeletalMesh(CharacterData.SkeletalMesh);
			GetMesh()->SetAnimInstanceClass(CharacterData.AnimBlueprint);
			GetCharacterMovement()->MaxWalkSpeed = Stat->GetTotalStat().MovementSpeed;

			CharacterSkillSet->InitializeSkills(InCharacterName);
			CharacterSpellSet->InitializeSpells(InCharacterName);
		}
	}
}

void AGOCharacterBase::ApplyStat(const FGOCharacterStat& BaseStat, const FGOCharacterStat& ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AGOCharacterBase::UpdateNicknameWidget(const FString& Nickname) {
	UE_LOG(LogTemp, Warning, TEXT("[TeamBattle] UpdateNicknameWidget: 000 %s"), *Nickname);

	if (StatsBarWidget) {
		UE_LOG(LogTemp, Warning, TEXT("[TeamBattle] UpdateNicknameWidget: 111 %s"), *Nickname);

		FText NicknameText = FText::FromString(Nickname);
		StatsBarWidget->Nickname->SetText(NicknameText);
		UE_LOG(LogTemp, Warning, TEXT("[TeamBattle] UpdateNicknameWidget: 222 %s"), *Nickname);
	}
}

void AGOCharacterBase::SetupCharacterWidget(UGOUserWidget* InUserWidget)
{
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
	}
	{
		//if (!InUserWidget)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("InUserWidget is null"));
		//	return;
		//}

		//StatsBarWidget = Cast<UGOStatsBarWidget>(InUserWidget);
		//if (!StatsBarWidget)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("StatsBarWidget is null"));
		//	return;
		//}

		//UGOHpBarWidget* HpBarWidget = Cast<UGOHpBarWidget>(StatsBarWidget->GetWidgetFromName(TEXT("PbHpBar")));
		//UGOManaBarWidget* ManaBarWidget = Cast<UGOManaBarWidget>(StatsBarWidget->GetWidgetFromName(TEXT("PbManaBar")));

		//if (!HpBarWidget || !ManaBarWidget)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("HpBarWidget or ManaBarWidget is null"));
		//	return;
		//}

		//// Binding stat changes to the widgets
		//Stat->OnHpChanged.AddUObject(HpBarWidget, &UGOHpBarWidget::UpdateHpBar);
		//Stat->OnManaChanged.AddUObject(ManaBarWidget, &UGOManaBarWidget::UpdateManaBar);

		//// Set initial values
		//HpBarWidget->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());
		//ManaBarWidget->UpdateManaBar(Stat->GetCurrentMana(), Stat->GetMaxMana());
	}

	//AGOPlayerState* PS = GetPlayerState<AGOPlayerState>();

	//FText Nickname = FText::FromString(PS->SelectedHero.PlayerName);
	//StatsBarWidget->Nickname->SetText(Nickname);

	StatsBarWidget = Cast<UGOStatsBarWidget>(InUserWidget);
	UE_LOG(LogTemp, Warning, TEXT("[TeamBattle] UpdateNicknameWidget: -1 %s"), *StatsBarWidget->GetName());

	if (!InUserWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("InUserWidget is null"));
		return;
	}

	StatsBarWidget = Cast<UGOStatsBarWidget>(InUserWidget);
	if (!StatsBarWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("StatsBarWidget is null"));
		return;
	}

	HpBarWidget = Cast<UGOHpBarWidget>(StatsBarWidget->GetWidgetFromName(TEXT("PbHpBar")));
	ManaBarWidget = Cast<UGOManaBarWidget>(StatsBarWidget->GetWidgetFromName(TEXT("PbManaBar")));

	if (!HpBarWidget || !ManaBarWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("HpBarWidget or ManaBarWidget is null"));
		return;
	}

	Stat->OnHpChanged.AddUObject(HpBarWidget, &UGOHpBarWidget::UpdateHpBar);
	Stat->OnManaChanged.AddUObject(ManaBarWidget, &UGOManaBarWidget::UpdateManaBar);

	HpBarWidget->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());
	ManaBarWidget->UpdateManaBar(Stat->GetCurrentMana(), Stat->GetMaxMana());
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

void AGOCharacterBase::SkillAttackHitCheck()
{
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
	bIsDead = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(true);
    if (HpBar) // Ensure HpBar is not null
        HpBar->SetHiddenInGame(true);
    
    if (ManaBar) // Ensure ManaBar is not null
        ManaBar->SetHiddenInGame(true);

	if (StatsBar)
		StatsBar->SetHiddenInGame(true);

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

void AGOCharacterBase::SetStunned()
{
	bIsStunned = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayStunnedAnimation();
}

void AGOCharacterBase::PlayStunnedAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(StunnedMontage, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("[SetStunned] AGOCharacterBase::PlayStunnedAnimation()"));
}

void AGOCharacterBase::GetMana()
{
}

void AGOCharacterBase::NoMana()
{
	// TODO: No Mana
}

FVector AGOCharacterBase::GetBattleSocketLocation()
{
	return GetMesh()->GetSocketLocation(CharacterData.WeaponTipSocketName);
}

void AGOCharacterBase::HighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("HighlightActor start 000000000"));
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250.f);
	UE_LOG(LogTemp, Warning, TEXT("HighlightActor end 000000000"));
}

void AGOCharacterBase::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	UE_LOG(LogTemp, Warning, TEXT("UnHighlightActor 000000000"));
}

bool AGOCharacterBase::GetIsDead()
{
	return bIsDead;
}

bool AGOCharacterBase::GetIsStunned()
{
	return bIsStunned;
}
