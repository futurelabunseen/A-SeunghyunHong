// Fill out your copyright notice in the Description page of Project Settings.


#include "GOCharacterStatComponent.h"
#include "GameData/GOGameSingleton.h"
#include "GameData/GOGameSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "GuardiansOrders/GuardiansOrders.h"

UGOCharacterStatComponent::UGOCharacterStatComponent()
{
	// CurrentCharacterType = 3;

	bWantsInitializeComponent = true;

	// 액터 컴포넌트 리플리케이트
	SetIsReplicated(true);

	ConstructorHelpers::FObjectFinder<UDataTable> CharacterStatDataObj(TEXT("DataTable'/Game/GameData/CharacterStatDataTable/GOCharacterStatTable.GOCharacterStatTable'"));
	if (CharacterStatDataObj.Succeeded())
	{
		CharacterStatDataTable = CharacterStatDataObj.Object;
	}
}

void UGOCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	// ResetStat();

	OnStatChanged.AddUObject(this, &UGOCharacterStatComponent::SetNewMaxHp);
	OnStatChanged.AddUObject(this, &UGOCharacterStatComponent::SetNewMaxMana);
}

void UGOCharacterStatComponent::SetCharacterStat(FName InCharacterName)
{
	static const FString ContextString(TEXT("Character Stat Data Lookup"));
	if (!CharacterStatDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterStatDataTable is not initialized."));
		return;
	}
	FGOCharacterStat* CharacterStatDataRow = CharacterStatDataTable->FindRow<FGOCharacterStat>(InCharacterName, ContextString);
	if (CharacterStatDataRow)
	{
		SetBaseStat(*CharacterStatDataRow);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character stats not found for %s."), *InCharacterName.ToString());
	}
	//CurrentCharacterType = FMath::Clamp(InNewCharacterType, 1, UGOGameSingleton::Get().CharacterMaxCnt);
	//SetBaseStat(UGOGameSingleton::Get().GetCharacterStat(CurrentCharacterType));

	//UGOGameSubsystem* GameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGOGameSubsystem>();
	//if (!GameSubsystem)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("GameSubsystem이 유효하지 않습니다."));
	//	return;
	//}
	//CurrentCharacterType = FMath::Clamp(InNewCharacterType, 1, GameSubsystem->CharacterMaxCnt);
	//SetBaseStat(GameSubsystem->GetCharacterStat(CurrentCharacterType));
	//if (!(BaseStat.MaxHp > 0.0f && BaseStat.MaxMana > 0.0f))
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Character stats for type %d are not initialized properly."), CurrentCharacterType);
	//	// Set default or fallback values
	//	BaseStat.MaxHp = 100.0f;  // Default value if not set
	//	BaseStat.MaxMana = 100.0f;  // Default value if not set
	//}
	//check(BaseStat.MaxHp > 0.0f && BaseStat.MaxMana > 0.0f);
}

float UGOCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp-ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

float UGOCharacterStatComponent::UseSkill(float SkillManaCost)
{
	const float PrevMana = CurrentMana;
	const float ManaCost = FMath::Clamp<float>(SkillManaCost, 0, SkillManaCost);
	SetMana(PrevMana - ManaCost);
	if (CurrentMana <= KINDA_SMALL_NUMBER)
	{
		// 스킬 사용 불가
		OnManaZero.Broadcast();
	}

	return ManaCost;
}

void UGOCharacterStatComponent::SetMana(float NewMana)
{
	CurrentMana = FMath::Clamp<float>(NewMana, 0.0f, MaxMana);
	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UGOCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UGOCharacterStatComponent::BeginPlay()
{
	GO_SUBLOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::BeginPlay();
	ResetStat();
	GetWorld()->GetTimerManager().SetTimer(ManaRegenerationTimer, this, &UGOCharacterStatComponent::RegenerateMana, 1.0f, true, 1.0f);
	GetWorld()->GetTimerManager().SetTimer(HpRegenerationTimer, this, &UGOCharacterStatComponent::RegenerateHp, 1.0f, true, 1.0f);
}

void UGOCharacterStatComponent::ReadyForReplication()
{
	GO_SUBLOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::ReadyForReplication();
}

void UGOCharacterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGOCharacterStatComponent, CurrentHp);
	DOREPLIFETIME(UGOCharacterStatComponent, MaxHp);
	DOREPLIFETIME(UGOCharacterStatComponent, CurrentMana);
	DOREPLIFETIME(UGOCharacterStatComponent, MaxMana);
	DOREPLIFETIME_CONDITION(UGOCharacterStatComponent, BaseStat, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UGOCharacterStatComponent, ModifierStat, COND_OwnerOnly);
}

void UGOCharacterStatComponent::SetNewMaxHp(const FGOCharacterStat& InBaseStat, const FGOCharacterStat& InModifierStat)
{
	float PrevMaxHp = MaxHp;
	MaxHp = GetTotalStat().MaxHp;
	if (PrevMaxHp != MaxHp)
	{
		OnHpChanged.Broadcast(CurrentHp, MaxHp);
	}
}

void UGOCharacterStatComponent::SetNewMaxMana(const FGOCharacterStat& InBaseStat, const FGOCharacterStat& InModifierStat)
{
	float PrevMaxMana= MaxMana;
	MaxMana = GetTotalStat().MaxMana;
	if (PrevMaxMana != MaxMana)
	{
		OnManaChanged.Broadcast(CurrentMana, MaxMana);
	}
}

void UGOCharacterStatComponent::RegenerateHp()
{
	if (CurrentHp < MaxHp)
	{
		CurrentHp = FMath::Min(CurrentHp + BaseStat.HpRegenerationRate, MaxHp);
		OnHpChanged.Broadcast(CurrentHp, MaxHp);
	}
}

void UGOCharacterStatComponent::RegenerateMana()
{
	if (CurrentMana < MaxMana)
	{
		CurrentMana = FMath::Min(CurrentMana + BaseStat.ManaRegenerationRate, MaxMana);
		OnManaChanged.Broadcast(CurrentMana, MaxMana);
	}
}

void UGOCharacterStatComponent::OnRep_CurrentHp()
{
	GO_SUBLOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	OnHpChanged.Broadcast(CurrentHp, MaxHp);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}

void UGOCharacterStatComponent::OnRep_MaxHp()
{
	GO_SUBLOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UGOCharacterStatComponent::OnRep_BaseStat()
{
	GO_SUBLOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	OnStatChanged.Broadcast(BaseStat, ModifierStat);
}

void UGOCharacterStatComponent::OnRep_ModifierStat()
{
	GO_SUBLOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	OnStatChanged.Broadcast(BaseStat, ModifierStat);
}

void UGOCharacterStatComponent::OnRep_CurrentMana()
{
	GO_SUBLOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));

	OnManaChanged.Broadcast(CurrentMana, MaxMana);
	if (CurrentMana <= KINDA_SMALL_NUMBER)
	{
		OnManaZero.Broadcast();
	}
}

void UGOCharacterStatComponent::OnRep_MaxMana()
{
	GO_SUBLOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UGOCharacterStatComponent::ResetStat()
{
	// SetCharacterStat(CurrentCharacterType);
	MaxHp = BaseStat.MaxHp;
	MaxMana = BaseStat.MaxMana;
	SetHp(MaxHp);
	SetMana(MaxMana);
}
