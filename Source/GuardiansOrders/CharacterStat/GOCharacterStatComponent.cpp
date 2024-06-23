// Fill out your copyright notice in the Description page of Project Settings.


#include "GOCharacterStatComponent.h"
#include "GameData/GOGameSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "GuardiansOrders/GuardiansOrders.h"
#include <Kismet/GameplayStatics.h>
#include <Interface/GOStateInterface.h>

UGOCharacterStatComponent::UGOCharacterStatComponent()
{
	// CurrentCharacterType = 3;

	bWantsInitializeComponent = true;
}

void UGOCharacterStatComponent::InitializeComponent()
{
	// 액터 컴포넌트 리플리케이트
	SetIsReplicated(true);

	Super::InitializeComponent();
	// ResetStat();

	OnStatChanged.AddUObject(this, &UGOCharacterStatComponent::SetNewMaxHp);
	OnStatChanged.AddUObject(this, &UGOCharacterStatComponent::SetNewMaxMana);
}

void UGOCharacterStatComponent::SetCharacterStat(FName InCharacterName)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;

	// Retrieve the subsystem from the game instance.
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();
	if (GOGameInstance)
	{
		FGOCharacterStat* CharacterStatDataRow = GOGameInstance->GetCharacterStatData(InCharacterName);
		if (CharacterStatDataRow)
		{
			SetBaseStat(*CharacterStatDataRow);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Character stats not found for %s."), *InCharacterName.ToString());
		}
	}
}

void UGOCharacterStatComponent::HealHp(float HealHpAmount)
{
	if(HealHpAmount == 0.f)
		CurrentHp = FMath::Clamp(CurrentHp + CurrentHp * 0.2, 0, GetTotalStat().MaxHp);
	if(HealHpAmount > 0.f)
		CurrentHp = FMath::Clamp(CurrentHp + HealHpAmount, 0, GetTotalStat().MaxHp);
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UGOCharacterStatComponent::ServerHealHp_Implementation(float HealHpAmount)
{
	HealHp(HealHpAmount);
	UE_LOG(LogTemp, Log, TEXT("ServerHealHp_Implementation "));
}

bool UGOCharacterStatComponent::ServerHealHp_Validate(float HealAmount)
{
	return true;  // 추가적인 유효성 검사 로직이 필요할 경우 이곳에 구현
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

	// for Skill Slot Widget
	UGOCharacterStatComponentOnManaChangedDelegate.Broadcast(CurrentMana);
	UE_LOG(LogTemp, Warning, TEXT("[UGOCharacterStatComponent::SetMana] CurrentMana is  %f"), CurrentMana);

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
	//// 액터 컴포넌트 리플리케이트
	//SetIsReplicated(true);
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
	// if(bIsDead != false)
	if (AActor* Owner = GetOwner())
	{
		if (IGOStateInterface* GOStateInterface = Cast<IGOStateInterface>(Owner))
		{
			if (GOStateInterface->GetIsDead() != false)
			{
				return;
			}
			else if (GOStateInterface->GetIsDead() == false) {
				if (CurrentHp < MaxHp)
				{
					UE_LOG(LogTemp, Warning, TEXT("[GOStateInterface->GetIsDead()]  RegenerateHp %d"), GOStateInterface->GetIsDead());

					CurrentHp = FMath::Min(CurrentHp + BaseStat.HpRegenerationRate, MaxHp);
					OnHpChanged.Broadcast(CurrentHp, MaxHp);
					UE_LOG(LogTemp, Warning, TEXT("[debuggung] RegenerateHp() called ended"));
				}
			}
		}
	}

}

void UGOCharacterStatComponent::RegenerateMana()
{	
	// if(bIsDead != false)
	if (AActor* Owner = GetOwner())
	{
		if (IGOStateInterface* GOStateInterface = Cast<IGOStateInterface>(Owner))
		{
			if (GOStateInterface->GetIsDead() != false)
			{
				return;
			}
			else if (GOStateInterface->GetIsDead() == false) {
				if (CurrentMana < MaxMana)
				{
					UE_LOG(LogTemp, Warning, TEXT("[GOStateInterface->GetIsDead()] RegenerateMana %d"), GOStateInterface->GetIsDead());

					CurrentMana = FMath::Min(CurrentMana + BaseStat.ManaRegenerationRate, MaxMana);
					OnManaChanged.Broadcast(CurrentMana, MaxMana);
					UGOCharacterStatComponentOnManaChangedDelegate.Broadcast(CurrentMana);
				}
			}
		}
	}
}

void UGOCharacterStatComponent::OnRep_CurrentHp()
{
	GO_SUBLOG(LogGONetwork, Log, TEXT("%s"), TEXT("Begin"));
	UE_LOG(LogTemp, Warning, TEXT("[debuggung] OnRep_CurrentHP() called CurrentHp: %d"), CurrentHp);

	OnHpChanged.Broadcast(CurrentHp, MaxHp);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	UE_LOG(LogTemp, Warning, TEXT("[debuggung] OnRep_CurrentHP() called ended CurrentHp: %d"), CurrentHp);

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

FGOCharacterStat UGOCharacterStatComponent::GetTotalStat() const
{
	return BaseStat + ModifierStat;
}

void UGOCharacterStatComponent::IncreaseBaseDamage(float Amount)
{
	float OldBaseDamage = BaseStat.BaseDamage;
	BaseStat.BaseDamage += Amount;
	OnRep_BaseStat();  // Manually trigger the replication function
	//OnStatIncreased.Broadcast(OldBaseDamage, Amount, BaseStat.BaseDamage);
}


void UGOCharacterStatComponent::LogStatChange(const FString& Context)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] BaseDamage: %d"), *Context, BaseStat.BaseDamage);
}

