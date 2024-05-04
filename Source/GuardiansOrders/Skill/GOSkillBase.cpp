// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillBase.h"
#include "Net/UnrealNetwork.h"
#include "GameData/GOGameSubsystem.h"
#include "Subsystems/SubsystemCollection.h"
#include "Kismet/GameplayStatics.h"

UGOSkillBase::UGOSkillBase()
{

}

void UGOSkillBase::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UGOSkillBase::SetSkillOwner(AActor* NewOwner)
{
	SkillOwnerCharacter = NewOwner;
}

void UGOSkillBase::InitializeSkill(FName InSkillName)
{
	UE_LOG(LogTemp, Warning, TEXT("UGOSkillBase::InitializeSkill InSkillName: %s"), InSkillName);

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;

	// Retrieve the subsystem from the game instance.
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();
	if (GOGameInstance)
	{
		FGOSkillData* SkillDataRow = GOGameInstance->GetSkillData(InSkillName);
		SkillData = *SkillDataRow;

		FGOSkillStat* SkillStatRow = GOGameInstance->GetSkillStatData(SkillDataRow->SkillStatName);

		if (SkillStatRow)
		{
			SkillStat = *SkillStatRow;
			UE_LOG(LogTemp, Warning, TEXT("SkillStat = *SkillStatRow; is called."));
		}
	}
	
}

void UGOSkillBase::StartCast()
{
	if (IsCastable() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill is not castable due to cooldown or other conditions."));
		return;
	}

	bIsCasting = true;
	SetCoolDownTimer();
}

void UGOSkillBase::UpdateCast(float DeltaTime)
{
	if (CoolDownTimer > 0.0f)
	{
		CoolDownTimer -= DeltaTime;
		if (CoolDownTimer <= 0.0f)
		{
			CoolDownTimer = 0.0f;
			bIsCastable = true;
		}
		OnCooldownUpdated.Broadcast(CoolDownTimer);
		UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOSkillBase::UpdateCast] is called. %d"), CoolDownTimer);
	}
}

void UGOSkillBase::Activate()
{
	// ��ų ȿ�� �ߵ� ����, ��: ����� ó��, ���� ȿ�� ���� ��
	UE_LOG(LogTemp, Log, TEXT("[UGOSkillBase::Activate()] Skill %s activated."), *SkillData.SkillName);
}

void UGOSkillBase::FinishCast()
{
	// bIsCasting = false;
	bIsCastable = false; // ��ٿ��� ����ǹǷ� �ٽ� ĳ��Ʈ�� �� ����
	SetCoolDownTimer();  // ��ٿ� Ÿ�̸� �缳��
	UE_LOG(LogTemp, Log, TEXT("[SkillBarUI UGOSkillBase::FinishCast()] is called "));

}

void UGOSkillBase::InterruptedCast()
{
	// bIsCasting = false;
	// �ʿ��� ��� ĳ���� �ߴ� ó��, ��: �ִϸ��̼� �ߴ�, ȿ�� ���� ��
	UE_LOG(LogTemp, Log, TEXT("Skill casting interrupted."));
}

void UGOSkillBase::ActivateEffect()
{

}