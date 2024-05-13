// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSpellBase.h"
#include "GameData/GOGameSubsystem.h"
#include "Subsystems/SubsystemCollection.h"
#include "Kismet/GameplayStatics.h"

UGOSpellBase::UGOSpellBase()
{
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
}

void UGOSpellBase::UpdateCast(float DeltaTime)
{
}

void UGOSpellBase::Activate()
{
}

void UGOSpellBase::FinishCast()
{
}

void UGOSpellBase::InterruptedCast()
{
}

void UGOSpellBase::ActivateEffect()
{
}
