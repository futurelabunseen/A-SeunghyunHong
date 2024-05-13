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
		// 이게 테이블을 직접 참조하는 것이다..! 내가 쓸 구조체로 만들어서 복사해서 써야 한다.
		// 이 클래스 인스턴스에서 전용으로 쓸 구조체를 만들어서 써야 한다!!
		// 안그러면 테이블 값이 바뀔 수 있다. 
		FGOSpellStat* SpellStatRow = GOGameInstance->GetSpellStatData(SpellDataRow->SpellStatName);

		if (SpellStatRow)
		{
			SpellStat = *SpellStatRow; // 이게 테이블을 직접 참조하는 것이다..!
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
