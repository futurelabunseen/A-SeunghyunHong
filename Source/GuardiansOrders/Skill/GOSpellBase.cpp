// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSpellBase.h"
#include "GameData/GOGameSubsystem.h"
#include "Subsystems/SubsystemCollection.h"
#include "Kismet/GameplayStatics.h"

UGOSpellBase::UGOSpellBase()
{
	bTickable = true;
	bTickableWhenPaused = false;
}

void UGOSpellBase::InitializeSpell(FName InSpellName)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;

	//// Retrieve the subsystem from the game instance.
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();
	if (GOGameInstance)
	{
		FGOSpellData* SpellDataRow = GOGameInstance->GetSpellData(InSpellName);
		SpellData = *SpellDataRow;
		// �̰� ���̺��� ���� �����ϴ� ���̴�..! ���� �� ����ü�� ���� �����ؼ� ��� �Ѵ�.
		// �� Ŭ���� �ν��Ͻ����� �������� �� ����ü�� ���� ��� �Ѵ�!!
		// �ȱ׷��� ���̺� ���� �ٲ� �� �ִ�. 
		FGOSpellStat* SpellStatRow = GOGameInstance->GetSpellStatData(SpellDataRow->SpellStatName);

		if (SpellStatRow)
		{
			SpellStat = *SpellStatRow; // �̰� ���̺��� ���� �����ϴ� ���̴�..!
			UE_LOG(LogTemp, Warning, TEXT("SpellStat = *SpellStatRow; is called."));
		}
	}
}

void UGOSpellBase::StartCast()
{
	bIsCasting = true;
	bIsCastable = false;
	SetIsOnCoolTime(true);
	SetCoolDownTimer();
	if (bIsCasting == true)
		UGOSpellBaseFIsOnCooldown.Broadcast(GetIsOnCoolTime());  // ��ٿ� ���� �� ��� UI ������Ʈ

	UE_LOG(LogTemp, Warning, TEXT("newspell :StartCast CoolDownTimer %f"), CoolDownTimer);

}

void UGOSpellBase::UpdateCast(float DeltaTime)
{
}

void UGOSpellBase::Activate()
{
}

void UGOSpellBase::FinishCast()
{
	bIsCasting = false;
	bIsCastable = true;
	SetCoolDownTimer();  // ��ٿ� Ÿ�̸� �缳�� �Ф�....
	SetIsOnCoolTime(false);
}

void UGOSpellBase::InterruptedCast()
{
}

void UGOSpellBase::ActivateEffect()
{
}

void UGOSpellBase::Tick(float DeltaTime)
{
	if (bIsCasting == true)
	{
		CoolDownTimer -= DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("newspell UGOSpellBase::Tick --"));
		if (CoolDownTimer <= 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("newspell UGOSpellBase::Tick <<"));

			FinishCast();
		}
	}
}

bool UGOSpellBase::IsTickable() const
{
	return bTickable;
}

bool UGOSpellBase::IsTickableInEditor() const
{
	return bTickable;
}

bool UGOSpellBase::IsTickableWhenPaused() const
{
	return bTickableWhenPaused;
}

TStatId UGOSpellBase::GetStatId() const
{
	return TStatId();
}

UWorld* UGOSpellBase::GetWorld() const
{
	return GetOuter()->GetWorld();
}
