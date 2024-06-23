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
	bIsCasting = true;
	bIsCastable = false;
	SetIsOnCoolTime(true);
	SetCoolDownTimer();
	if (bIsCasting == true)
		UGOSpellBaseFIsOnCooldown.Broadcast(GetIsOnCoolTime());  // 쿨다운 시작 시 즉시 UI 업데이트

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
	SetCoolDownTimer();  // 쿨다운 타이머 재설정 ㅠㅠ....
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
